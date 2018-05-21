#include "mcv_platform.h"
#include "game_core_skeleton.h"
#include "render/mesh/mesh_loader.h"
#include "utils/data_saver.h"
#include "cal3d2engine.h"
#include "ctes.h"

#pragma comment(lib, "cal3d.lib" )

class CGameCoreSkeletonResourceClass : public CResourceClass {
public:
    CGameCoreSkeletonResourceClass() {
        class_name = "Skeletons";
        extensions = { ".skeleton" };
    }
    IResource* create(const std::string& name) const override {
        dbg("Creating skelton resource %s\n", name.c_str());
        CGameCoreSkeleton* res = new CGameCoreSkeleton(name);
        bool is_ok = res->create(name);
        assert(is_ok);
        return res;
    }
};

// A specialization of the template defined at the top of this file
// If someone class getResourceClassOf<CGameCoreSkeletonResourceClass>, use this function:
template<>
const CResourceClass* getResourceClassOf<CGameCoreSkeleton>() {
    static CGameCoreSkeletonResourceClass the_resource_class;
    return &the_resource_class;
}

struct TSkinVertex {
    VEC3 pos;
    VEC3 normal;
    VEC2 uv;
    VEC2 uv2;
    VEC4 tangent;
    uint8_t bone_ids[4];
    uint8_t bone_weights[4];    // 0.255   -> 0..1
};

void showCoreBoneRecursive(CalCoreSkeleton* core_skel, int bone_id) {
    assert(core_skel);
    if (bone_id == -1) return;
    CalCoreBone* cb = core_skel->getCoreBone(bone_id);
    char buf[128];
    sprintf(buf, "%s [Id:%d]", cb->getName().c_str(), bone_id);
    ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Appearing);
    if (ImGui::TreeNode(buf)) {
        auto children = cb->getListChildId();
        for (auto it : children) {
            showCoreBoneRecursive(core_skel, it);
        }
        ImGui::TreePop();
    }
}

void CGameCoreSkeleton::debugInMenu() {
    auto* core_skel = getCoreSkeleton();

    if (ImGui::TreeNode("Hierarchy Bones")) {
        auto& core_bones = core_skel->getVectorRootCoreBoneId();
        for (auto& bone_id : core_bones)
            showCoreBoneRecursive(getCoreSkeleton(), bone_id);
        ImGui::TreePop();
    }
    ImGui::DragFloat("Debug Bones Scale", &bone_ids_debug_scale, 0.01f, 0.1f, 15.0f);

    if (ImGui::TreeNode("LookAt Corrections")) {
        for (auto& it : lookat_corrections) {
            ImGui::PushID(&it);
            it.debugInMenu();
            ImGui::PopID();
        }
        ImGui::TreePop();
    }
}

bool CGameCoreSkeleton::convertCalCoreMesh2RenderMesh(CalCoreMesh* cal_mesh,
                                                      const std::string& ofilename) {
    CMemoryDataSaver mds_vtxs;
    CMemoryDataSaver mds_idxs;

    auto nbones = this->getCoreSkeleton()->getVectorCoreBone().size();

    auto nsubmeshes = cal_mesh->getCoreSubmeshCount();

    // Compute total vertexs counting all materials
    int  total_vtxs = 0;
    int  total_faces = 0;
    for (int idx_sm = 0; idx_sm < nsubmeshes; ++idx_sm) {
        CalCoreSubmesh* cal_sm = cal_mesh->getCoreSubmesh(idx_sm);
        total_vtxs += cal_sm->getVertexCount();
        total_faces += cal_sm->getFaceCount();
    }

    // For each submesh ( each material...);
    int  acc_vtxs = 0;
    for (int idx_sm = 0; idx_sm < nsubmeshes; ++idx_sm) {
        CalCoreSubmesh* cal_sm = cal_mesh->getCoreSubmesh(idx_sm);
        cal_sm->enableTangents(0, true);

        // Copy The vertexs
        auto& cal_vtxs = cal_sm->getVectorVertex();

        auto num_vtxs = cal_sm->getVertexCount();

        // Access to the first texture coordinate set
        auto& cal_all_uvs = cal_sm->getVectorVectorTextureCoordinate();
        if (cal_all_uvs.empty()) {
            //fatal("The skin mesh does not have texture coords!\n");
            //return false;
            // Create some fake/empty tex coords
            cal_all_uvs.resize(1);
            cal_all_uvs[0].resize(num_vtxs);
        }
        auto& cal_uvs0 = cal_all_uvs[0];

        //hacer if
        auto& cal_all_tangents = cal_sm->getVectorVectorTangentSpace();
        std::vector<CalCoreSubmesh::TangentSpace> cal_tangents;
        if (cal_all_tangents.size() > 0) {
            cal_tangents = cal_all_tangents[0];
            assert(cal_all_tangents.size() > 0);
            assert(cal_tangents.size() == num_vtxs);
        }

        // Process the vtxs
        for (int vid = 0; vid < num_vtxs; ++vid) {
            CalCoreSubmesh::Vertex& cal_vtx = cal_vtxs[vid];

            // Prepare a clean vertexs
            TSkinVertex skin_vtx;
            memset(&skin_vtx, 0x00, sizeof(TSkinVertex));

            // Pos & Normal
            skin_vtx.pos = Cal2DX(cal_vtx.position);
            skin_vtx.normal = Cal2DX(cal_vtx.normal);

            // Texture coords
            skin_vtx.uv.x = cal_uvs0[vid].u;//poner a 1, 0, en el else
            skin_vtx.uv.y = cal_uvs0[vid].v;
            if (cal_all_tangents.size() > 0) {
                skin_vtx.uv2.x = cal_uvs0[vid].u;
                skin_vtx.uv2.y = cal_uvs0[vid].v;

                skin_vtx.tangent.x = cal_tangents[vid].tangent.x;
                skin_vtx.tangent.y = cal_tangents[vid].tangent.y;
                skin_vtx.tangent.z = cal_tangents[vid].tangent.z;
                skin_vtx.tangent.w = cal_tangents[vid].crossFactor;
            }
            else {
                skin_vtx.uv2.x = 0;
                skin_vtx.uv2.y = 1;

                skin_vtx.tangent.x = 1;
                skin_vtx.tangent.y = 0;
                skin_vtx.tangent.z = 0;
                skin_vtx.tangent.w = 0;
            }

            // Weights...
            int total_weight = 0;
            for (size_t ninfluence = 0;
                 ninfluence < cal_vtx.vectorInfluence.size() &&
                 ninfluence < 4;
                 ++ninfluence) {
                auto cal_influence = cal_vtx.vectorInfluence[ninfluence];
                assert(cal_influence.boneId < MAX_SUPPORTED_BONES);
                skin_vtx.bone_ids[ninfluence] = (uint8_t)(cal_influence.boneId);
                assert(skin_vtx.bone_ids[ninfluence] < nbones);

                // Convert cal3d influence from 0..1 to a char from 0..255
                skin_vtx.bone_weights[ninfluence] = (uint8_t)(255.f * cal_influence.weight);

                total_weight += skin_vtx.bone_weights[ninfluence];
            }

            // Confirm the sum of rounded weights are still 1  = > 255
            int err = 255 - total_weight;
            if (err != 0) {
                assert(err > 0);
                skin_vtx.bone_weights[0] += err;
            }

            mds_vtxs.write(skin_vtx);
        }

        // Process the faces
        auto num_faces = cal_sm->getFaceCount();
        const auto& cal_faces = cal_sm->getVectorFace();
        for (auto& face : cal_faces) {
            static int lut[3] = { 0,2,1 };
            for (int i = 0; i < 3; ++i) {
                int my_i = lut[i];      // swap face culling

                uint32_t vertex_id = face.vertexId[my_i];

                // The second ground of vertexs, have the face_id not starting in the zero
                // it starts in the previous count of vertex we have defined
                // VertexSubMesh0... | VertexSubMesh1 ... |...
                vertex_id += acc_vtxs;

                // Cal is defined to be 32 bits per index
                if (total_vtxs > 65535) {
                    mds_idxs.write((uint32_t)vertex_id);   // 4 bytes per index
                }
                else {
                    mds_idxs.write((uint16_t)vertex_id);   // 2 bytes per index
                }
            }
        }

        // Keep the total accumulated vertexs
        acc_vtxs += num_vtxs;
    }

    TMeshLoader mesh_io;
    auto& header = mesh_io.header;
    header.bytes_per_idx = total_vtxs > 65535 ? sizeof(uint32_t) : sizeof(uint16_t);
    header.bytes_per_vtx = sizeof(TSkinVertex);
    header.num_indices = total_faces * 3;
    header.num_vertexs = total_vtxs;
    header.primitive_type = CRenderMesh::TRIANGLE_LIST;
    strcpy(header.vertex_type_name, "PosNUvUvTSkin");

    mesh_io.vtxs = mds_vtxs.buffer;
    mesh_io.idxs = mds_idxs.buffer;

    // Map each subgroup from cal3d to one render from us
    // mesh_io.subgroups.resize(nsubmeshes);
    // ...

    CFileDataSaver fds(ofilename.c_str());
    mesh_io.save(fds);

    return true;
}

bool CGameCoreSkeleton::create(const std::string& res_name) {
    // Load an initial scene
    json json = loadJson(res_name);

    std::string name = json["name"];
    root_path = "data/skeletons/" + name + "/";

    //CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS | LOADER_INVERT_V_COORD);
    CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS);

    // Read the core skeleton
    std::string csf = root_path + name + ".csf";
    bool is_ok = loadCoreSkeleton(csf);
    if (!is_ok) return false;

    // Check if there is already a .mesh
    if (json.count("meshes")) {
        auto& meshes = json["meshes"];
        assert(meshes.is_array());
        for (size_t i = 0; i < meshes.size(); ++i) {
            if (meshes[i].is_string()) {
                std::string meshName = meshes[i];
                std::string cmf = root_path + meshName + ".cmf";
                int mesh_id = loadCoreMesh(cmf);
                if (mesh_id < 0)
                    return false;
                std::string skin_mesh_file = root_path + meshName + ".mesh";
                convertCalCoreMesh2RenderMesh(getCoreMesh(mesh_id), skin_mesh_file);
            }
        }
    }
    else {
        std::string cmf = root_path + name + ".cmf";
        if (fileExists(cmf)) {
            int mesh_id = loadCoreMesh(cmf);
            if (mesh_id < 0)
                return false;
            std::string skin_mesh_file = root_path + name + ".mesh";
            convertCalCoreMesh2RenderMesh(getCoreMesh(mesh_id), skin_mesh_file);
            // Delete the cmf file
            // std::remove(cmf.c_str());
        }
    }

    // Read all anims
    auto& anims = json["anims"];
    for (auto it = anims.begin(); it != anims.end(); ++it) {
        assert(it->is_object());

        auto& anim = *it;
        std::string anim_name = anim["name"];
        std::string caf = root_path + anim_name + ".caf";
        int anim_id = loadCoreAnimation(caf, anim_name);
        if (anim_id < 0)
            return false;

        // read other metadata associated to the anim
        // ...
    }

    // Array of bone ids to debug (auto conversion from array of json to array of ints)
    if (json["bone_ids_to_debug"].is_array())
        bone_ids_to_debug = json["bone_ids_to_debug"].get< std::vector< int > >();

    // Read shared lookat correction set of bones
    if (json.count("lookat_corrections")) {
        auto& jcorrs = json["lookat_corrections"];
        assert(jcorrs.is_array());
        for (int i = 0; i < jcorrs.size(); ++i) {
            TBoneCorrection c;
            c.load(jcorrs[i]);
            // Resolve the bone_name to bone_id here
            c.bone_id = getCoreSkeleton()->getCoreBoneId(c.bone_name);
            lookat_corrections.push_back(c);
        }
    }

    // Load Ragdoll Core
    auto& jragdoll_core = json["ragdoll"];
    if (json["ragdoll"].is_array()) {
        for (auto& j : jragdoll_core) {
            if (!j.is_null()) {
                ragdoll_core.ragdoll_bone_cores.resize(ragdoll_core.ragdoll_bone_cores.size() + 1);
                TRagdollBoneCore& ragdoll_bone_core = ragdoll_core.ragdoll_bone_cores.back();
                ragdoll_bone_core.bone = j["bone"];
                ragdoll_bone_core.height = j["height"].get<float>();
                ragdoll_bone_core.radius = j["radius"].get<float>();

                ragdoll_bone_core.parent_bone = j["parent_bone"];
            }
        }

        for (auto& core_bone : ragdoll_core.ragdoll_bone_cores) {
            for (auto& parent_core_bone : ragdoll_core.ragdoll_bone_cores) {
                if (core_bone.parent_bone == parent_core_bone.bone) {
                    core_bone.parent_core = &parent_core_bone;
                    break;
                }
            }
        }
    }
    return true;
}
