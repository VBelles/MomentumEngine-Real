#include "mcv_platform.h"
#include "module_navmesh.h"

CModuleNavmesh::CModuleNavmesh(const std::string& name) : IModule(name) {
}

bool CModuleNavmesh::start() {
	//TODO map con todas las navmeshes
	navmesh = new CNavMesh();
	navmesh->create("data/solo_navmesh.bin");
	navQuery = new CNavMeshQuery(navmesh);
	return true;
}

void CModuleNavmesh::render() {
    if (CApp::get().isDebug()) {
        ImGui::Checkbox("Show navmesh", &showNavmesh);

        if (showNavmesh) {
            navmesh->render();
        }
    }
}

bool CModuleNavmesh::stop() {
	navmesh->destroy();
	SAFE_DELETE(navmesh);
	return true;
}

void CModuleNavmesh::update(float delta) {

}
