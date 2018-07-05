#include "mcv_platform.h"
#include "module_test_instancing.h"
#include "geometry/transform.h"
#include "entity/entity_parser.h"
#include "components/comp_transform.h"

float unitRandom() {
	return (float)rand() / (float)RAND_MAX;
}

float randomFloat(float vmin, float vmax) {
	return vmin + (vmax - vmin) * unitRandom();
}

bool CModuleTestInstancing::start() {
	{
		auto rmesh = Resources.get("data/meshes/GeoSphere001.instanced_mesh")->as<CRenderMesh>();
		// Remove cast and upcast to CRenderMeshInstanced
		instances_mesh = (CRenderMeshInstanced*)rmesh;
	}
	{
		auto rmesh = Resources.get("data/meshes/blood.instanced_mesh")->as<CRenderMesh>();
		blood_instances_mesh = (CRenderMeshInstanced*)rmesh;
	}
	{
		auto rmesh = Resources.get("data/meshes/particles.instanced_mesh")->as<CRenderMesh>();
		particles_instances_mesh = (CRenderMeshInstanced*)rmesh;
	}
	{
		auto rmesh = Resources.get("data/meshes/grass.instanced_mesh")->as<CRenderMesh>();
		grass_instances_mesh = (CRenderMeshInstanced*)rmesh;
		grass_instances_mesh->setInstancesData(grass_instances.data(), grass_instances.size(), sizeof(TGrassParticle));
	}

	auto& json = loadJson("data/instancing/grass.json");
	for (auto& mesh : json) {
		std::string type = mesh.value("type", "");
		if (type == "grass") {
			for (auto& data : mesh["data"]) {
				TGrassParticle new_instance;
				new_instance.pos = loadVEC3(data["pos"]);
				grass_instances.push_back(new_instance);
			}
		}
		else if (type == "geosphere") {
			for (auto& data : mesh["data"]) {
				TInstance new_instance;
				VEC3 pos = data.count("pos") ? loadVEC3(data["pos"]) : VEC3::Zero;
				VEC4 rot = data.count("rot") ? loadVEC4(data["rot"]) : VEC4::Zero;
				float scale = data.value("scale", 1.f);
				new_instance.world = MAT44::CreateScale(scale) * CTransform(pos, rot).asMatrix();
				instances.push_back(new_instance);
			}
		}
	}
	dbg("Instancing: %d grass\n", grass_instances.size());
	dbg("Instancing: %d geosphere\n", instances.size());
	grass_instances_mesh->setInstancesData(grass_instances.data(), grass_instances.size(), sizeof(TGrassParticle));
	instances_mesh->setInstancesData(instances.data(), instances.size(), sizeof(TInstance));


	return true;
}

void CModuleTestInstancing::update(float delta) {
	if (nGrassToInstance > 0) {
		for (int i = 0; i < nGrassToInstance; ++i) {
			TGrassParticle new_instance;
			new_instance.pos = VEC3(randomFloat(-sz, sz), 0, randomFloat(-sz, sz));
			grass_instances.push_back(new_instance);
		}
		grass_instances_mesh->setInstancesData(grass_instances.data(), grass_instances.size(), sizeof(TGrassParticle));
		nGrassToInstance = 0;
	}

	// Rotate the particles
	for (auto& p : particles_instances)
		p.angle += delta;

	// Move the instances in the cpu
	static float t = 0;
	t += delta;
	//for (auto& p : instances)
		//p.world = p.world * MAT44::CreateTranslation(VEC3(0, 0.1f * sin(t), 0));
	//instances_mesh->setInstancesData(instances.data(), instances.size(), sizeof(TInstance));

	blood_instances_mesh->setInstancesData(blood_instances.data(), blood_instances.size(), sizeof(TInstanceBlood));

	particles_instances_mesh->setInstancesData(particles_instances.data(), particles_instances.size(), sizeof(TRenderParticle));
}

void CModuleTestInstancing::render() {
	if (!CApp::get().isDebug()) return;
	if (ImGui::TreeNode("Instancing")) {

		// -- Creation params ----------------------
		static int num = 3;
		ImGui::DragFloat("Size", &sz, 0.01f, -50.f, 50.f);
		ImGui::DragInt("Num", &num, 0.1f, 1, 10);

		// ----------------------------------------------
		if (ImGui::TreeNode("Particles")) {
			ImGui::Text("Num Instances: %ld / %ld. GPU:%d", particles_instances.size(), particles_instances.capacity(), particles_instances_mesh->getVertexsCount());

			if (ImGui::Button("Add")) {
				for (int i = 0; i < num; ++i) {
					TRenderParticle new_instance;
					new_instance.scale_x = randomFloat(1.f, 5.f);
					new_instance.scale_y = new_instance.scale_x;
					new_instance.pos = VEC3(randomFloat(-sz, sz), new_instance.scale_x, randomFloat(-sz, sz));
					new_instance.nframe = randomFloat(0.f, 16.f);
					new_instance.angle = deg2rad(randomFloat(0, 360));
					new_instance.color.x = unitRandom();
					new_instance.color.y = unitRandom();
					new_instance.color.z = 1 - new_instance.color.x - new_instance.color.y;
					new_instance.color.w = 1;
					particles_instances.push_back(new_instance);
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Del") && !particles_instances.empty())
				particles_instances.pop_back();
			ImGui::TreePop();
		}

		// ----------------------------------------------
		if (ImGui::TreeNode("Spheres")) {
			ImGui::Text("Num Instances: %ld / %ld. GPU:%d", instances.size(), instances.capacity(), instances_mesh->getVertexsCount());

			if (ImGui::Button("Add")) {
				for (int i = 0; i < num; ++i) {
					TInstance new_instance;
					new_instance.world = MAT44::CreateTranslation(VEC3(randomFloat(-sz, sz), 0, randomFloat(-sz, sz)));
					instances.push_back(new_instance);
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Del") && !instances.empty())
				instances.pop_back();
			ImGui::TreePop();
		}

		// ----------------------------------------------
		if (ImGui::TreeNode("Grass")) {
			bool changed = false;
			ImGui::Text("Num Instances: %ld / %ld. GPU:%d", grass_instances.size(), grass_instances.capacity(), grass_instances_mesh->getVertexsCount());
			if (ImGui::Button("Add 100")) {
				nGrassToInstance = num * 100;
			}
			else {
				nGrassToInstance = 0;
			}
			ImGui::SameLine();
			/*if (ImGui::Button("Del") && !instances.empty()) {
				if (nGrassToInstance < instances.size())
					nGrassToInstance = instances.size();
				instances.resize(instances.size() - nGrassToInstance);
				changed = true;
			}*/
			ImGui::TreePop();

			// Update GPU with the new CPU
			//if (changed)
				//grass_instances_mesh->setInstancesData(grass_instances.data(), grass_instances.size(), sizeof(TGrassParticle));
		}

		// ----------------------------------------------
		if (ImGui::TreeNode("Blood")) {
			ImGui::Text("Num Instances: %ld / %ld. GPU:%d", blood_instances.size(), blood_instances.capacity(), blood_instances_mesh->getVertexsCount());
			if (ImGui::Button("Add")) {
				for (int i = 0; i < num; ++i) {
					TInstanceBlood new_instance;
					new_instance.world =
						MAT44::CreateRotationY(randomFloat((float)-M_PI, (float)M_PI))
						*
						MAT44::CreateScale(randomFloat(2.f, 10.f))
						*
						MAT44::CreateTranslation(VEC3(randomFloat(-sz, sz), 0, randomFloat(-sz, sz)));
					new_instance.color.x = unitRandom();
					new_instance.color.y = unitRandom();
					new_instance.color.z = 1 - new_instance.color.x - new_instance.color.y;
					new_instance.color.w = 1;
					blood_instances.push_back(new_instance);
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Del") && !blood_instances.empty())
				blood_instances.pop_back();
			if (ImGui::TreeNode("Instances")) {
				for (auto& p : blood_instances) {
					ImGui::PushID(&p);
					VEC3 scale, trans;
					QUAT rot;
					p.world.Decompose(scale, rot, trans);
					CTransform tmx;
					tmx.setRotation(rot);
					tmx.setPosition(trans);
					tmx.setScale(scale.x);
					if (tmx.debugInMenu())
						p.world = tmx.asMatrix();
					ImGui::ColorEdit4("Color", &p.color.x);
					ImGui::PopID();
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		// ----------------------------------------------
		if (ImGui::SmallButton("Make Teapots")) {
			std::string prefab = "data/prefabs/teapot_hi.prefab";
			for (int i = -num; i < num; ++i) {
				for (int j = -num; j < num; ++j) {
					VEC3 pos((float)i, 0, (float)j);
					TEntityParseContext ctx;
					parseScene(prefab, ctx);
					CEntity* e = ctx.entities_loaded[0];
					TCompTransform* t = e->get<TCompTransform>();
					t->setPosition(pos * sz);
				}
			}
		}

		ImGui::TreePop();
	}
}
