#include "mcv_platform.h"
#include "module_navmesh.h"

CModuleNavmesh::CModuleNavmesh(const std::string& name) : IModule(name) {
}

bool CModuleNavmesh::start() {
	return navmesh.create("data/solo_navmesh.bin");
}

void CModuleNavmesh::render() {
    if (CApp::get().showDebug) {
        ImGui::Checkbox("Show navmesh", &showNavmesh);

        if (showNavmesh) {
            navmesh.render();
        }
    }
}

bool CModuleNavmesh::stop() {
	navmesh.destroy();
	return true;
}

void CModuleNavmesh::update(float delta) {

}
