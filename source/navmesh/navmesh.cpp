#include "mcv_platform.h"
#include "navmesh.h"

class CNavMeshResourceClass : public CResourceClass {
public:
	CNavMeshResourceClass() {
        class_name = "Meshes";
        extensions = { ".mesh", ".instanced_mesh" };
    }
    IResource* create(const std::string& name) const override {
        CNavMesh* res = nullptr;
        if (name.find(".instanced_mesh") != std::string::npos)
            res = loadMeshInstanced(name);
        else res = loadMesh(name.c_str());
        return res;
    }
};

// A specialization of the template defined at the top of this file
// If someone class getResourceClassOf<CTexture>, use this function:
template<>
const CResourceClass* getResourceClassOf<CNavMesh>() {
    static CNavMeshResourceClass the_resource_class;
    return &the_resource_class;
}

bool CNavMesh::create() {
	// TODO si queremos generar navmesh ingame.

    return true;
}

bool CNavMesh::isValid() const {

}

void CNavMesh::destroy() {
}

void CNavMesh::activate() const {

}

void CNavMesh::debugInMenu() {

}

void CNavMesh::updateFromCPU(const void *new_cpu_data, size_t num_bytes_to_update) {

}
