#include "mcv_platform.h"
#include "module_instancing.h"
#include "geometry/transform.h"
#include "entity/entity_parser.h"

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
	for (auto& jTInstance : jInstances) {
		//Load mesh
		std::string meshName = jTInstance.value("mesh", "");
		//auto instanceMesh = (CRenderMeshInstanced*)Resources.get(meshName)->as<CRenderMesh>();
		//Load data
		//std::vector<TInstance> instancesData;
		for (auto& jData : jTInstance["data"]) {
			TInstance instanceData;
			VEC3 pos = jData.count("pos") ? loadVEC3(jData["pos"]) : VEC3::Zero;
			VEC4 rot = jData.count("rot") ? loadVEC4(jData["rot"]) : VEC4::Zero;
			float scale = jData.value("scale", 1.f);
			instanceData.world = MAT44::CreateScale(scale) * CTransform(pos, rot).asMatrix();
			instancesDataMap[meshName].push_back(instanceData);
		}
		//instanceMesh->setInstancesData(instancesData.data(), instancesData.size(), sizeof(TInstance));
	}
}

void CModuleInstancing::setAllInstancesData() {
	for (std::map<std::string, std::vector<TInstance>>::iterator iter = instancesDataMap.begin(); iter != instancesDataMap.end();) {
		auto instanceMesh = (CRenderMeshInstanced*)Resources.get((*iter).first)->as<CRenderMesh>();
		instanceMesh->setInstancesData((*iter).second.data(), (*iter).second.size(), sizeof(TInstance));
	}
}

void CModuleInstancing::update(float delta) {

}

void CModuleInstancing::render() {

}

