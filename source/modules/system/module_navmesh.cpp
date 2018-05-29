#include "mcv_platform.h"
#include "module_navmesh.h"

CModuleNavmesh::CModuleNavmesh(const std::string& name) : IModule(name) {
}

bool CModuleNavmesh::start() {
	navmesh.create("data/solo_navmesh.bin");
	//navQuery = CNavMeshQuery{ &navmesh };
	return true;
}

void CModuleNavmesh::render() {
	navmesh.render();
}

bool CModuleNavmesh::stop() {
	navmesh.destroy();
	return true;
}

void CModuleNavmesh::update(float delta) {

}
