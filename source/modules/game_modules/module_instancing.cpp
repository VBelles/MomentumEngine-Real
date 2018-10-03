#include "mcv_platform.h"
#include "module_instancing.h"
#include "geometry/transform.h"
#include "entity/entity_parser.h"
#include "components/comp_culling.h"

bool CModuleInstancing::start() {
	auto files = WindowsUtils::getAllFiles("data/instancing/", "*.json");
	for (auto& file : files) {
		auto& jInstances = loadJson(file);
		loadInstances(jInstances["TInstance"]);
	}
	setAllInstancesData();
	return true;
}

void CModuleInstancing::loadInstances(const json& jInstances) {
	static unsigned int id = 0;
	for (auto& jTInstance : jInstances) {
		std::string meshName = jTInstance.value("mesh", "");
		auto instanceMesh = (CRenderMeshInstanced*)Resources.get(meshName)->as<CRenderMesh>();
		assert(instanceMesh);
		//Load data
		for (auto& jData : jTInstance["data"]) {
			TInstance instanceData;
			VEC3 pos = jData.count("pos") ? loadVEC3(jData["pos"]) : VEC3::Zero;
			VEC4 rot = jData.count("rot") ? loadVEC4(jData["rot"]) : VEC4::Zero;
			float scale = jData.value("scale", 1.f);
			instanceData.world = MAT44::CreateScale(scale) * CTransform(pos, rot).asMatrix();
			instanceData.id = id;
			AABB aabb = instanceMesh->getAABB();
			aabb.Transform(aabb, instanceData.world);

			instancesDataMap[meshName].push_back({ instanceData, aabb });
			id++;
		}
	}
}

void CModuleInstancing::setAllInstancesData() {
	for (auto& p : instancesDataMap) {
		auto& instancesDataAABB = p.second;
		std::vector<TInstance> data;
		for (auto& instanceDataAABB : instancesDataAABB) {
			data.push_back(instanceDataAABB.data);
		}
		auto instanceMesh = (CRenderMeshInstanced*)Resources.get(p.first)->as<CRenderMesh>();
		instanceMesh->setInstancesData(data.data(), data.size(), sizeof(TInstance));
	}
}

void CModuleInstancing::update(float delta) {
	if (!culling) return;
	CEntity* cameraEntity = getEntityByName(GAME_CAMERA);
	TCompCulling* culling = cameraEntity->get<TCompCulling>();
	for (auto& p : instancesDataMap) {
		auto& instancesDataAABB = p.second;
		std::vector<TInstance> data;
		for (auto& instanceDataAABB : instancesDataAABB) {
			if (culling->planes.isVisible(&instanceDataAABB.aabb)) {
				data.push_back(instanceDataAABB.data);
			}
		}
		auto instanceMesh = (CRenderMeshInstanced*)Resources.get(p.first)->as<CRenderMesh>();
		instanceMesh->setInstancesData(data.data(), data.size(), sizeof(TInstance));
	}
}

void CModuleInstancing::render() {
	if (CApp::get().isDebug()) {
		if (ImGui::TreeNode("Instancing")) {
			if (ImGui::Checkbox("Culling", &culling)) {
				if (!culling) {
					setAllInstancesData();
				}
			}
			ImGui::TreePop();
		}
	}
}

