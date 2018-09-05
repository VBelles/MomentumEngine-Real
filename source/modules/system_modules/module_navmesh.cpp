#include "mcv_platform.h"
#include "module_navmesh.h"

CModuleNavmesh::CModuleNavmesh(const std::string& name) : IModule(name) {
}

bool CModuleNavmesh::start() {
	json jNavmeshes = loadJson("data/navMeshes/autoload_navMeshes.json");
	if (jNavmeshes.count("nav_meshes")) {
		for (auto it = jNavmeshes["nav_meshes"].begin(); it != jNavmeshes["nav_meshes"].end(); ++it) {
			json& j = it.value();
			std::string id = j.value("id", "");
			std::string file = j.value("file", "");

			if (navMeshes.find(id) == navMeshes.end()) {
				CNavMesh* navMesh = new CNavMesh();
				if (navMesh->create(file)) {
					CNavMeshQuery* navMeshQuery = new CNavMeshQuery(navMesh);
					navMeshes[id].first = navMesh;
					navMeshes[id].second = navMeshQuery;
					renderNavMesh[id] = false;
				}
				else {
					delete navMesh;
					dbg("NavMesh %s is invalid, discarded\n", id.c_str());
				}
			}
			else {
				dbg("NavMesh %s already exists, discarding this one\n", id.c_str());
			}
		}
	}

	return true;
}

void CModuleNavmesh::render() {
	if (CApp::get().isDebug()) {
		if (ImGui::TreeNode("Nav meshes")) {
			for (auto& pair : renderNavMesh) {
				ImGui::Checkbox(("Render navMesh " + pair.first).c_str(), &pair.second);

				if (pair.second) {
					navMeshes[pair.first].first->render();
				}
			}
			ImGui::TreePop();
		}
	}
}

bool CModuleNavmesh::stop() {
	for (auto& pair : navMeshes) {
		if (pair.second.first) {
			pair.second.first->destroy();
			safeDelete(pair.second.first);
			safeDelete(pair.second.second);
		}
	}
	navMeshes.clear();
	renderNavMesh.clear();
	return true;
}

void CModuleNavmesh::update(float delta) {

}

CNavMesh* CModuleNavmesh::getNavMesh(std::string id) {
	auto& it = navMeshes.find(id);
	if (it != navMeshes.end()) {
		return it->second.first;
	}
	else {
		return nullptr;
	}
}

CNavMeshQuery* CModuleNavmesh::getNavQuery(std::string id) {
	auto& it = navMeshes.find(id);
	if (it != navMeshes.end()) {
		return it->second.second;
	}
	else {
		return nullptr;
	}
}
