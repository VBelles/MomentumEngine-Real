#include "mcv_platform.h"
#include "comp_altar.h"
#include "components/comp_transform.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("altar", TCompAltar);

void TCompAltar::debugInMenu() {
}

void TCompAltar::registerMsgs() {
    DECL_MSG(TCompAltar, TMsgAllScenesCreated, onAllScenesCreated);
	DECL_MSG(TCompAltar, TMsgDestroy, onDestroy);
}

void TCompAltar::load(const json& j, TEntityParseContext& ctx) {
	if(j.count("spawn_positions")) {
		auto& positions = j["spawn_positions"];
		assert(positions.is_array());
		for (auto& position : positions) {
			VEC3 pos = loadVEC3(position);
			spawnPositions.push_back(pos);
		}
	}
	prefabToSpawn = j["prefab"].get<std::string>();
	activationEffectFile = j["activationEffectFile"].get<std::string>();
	
}

void TCompAltar::onAllScenesCreated(const TMsgAllScenesCreated & msg) {
}

void TCompAltar::onDestroy(const TMsgDestroy & msg) {
	//Parar tiempo, hacer aparecer crisálidas, pinchar cámaras, (enseñar ui de crisálidas?)
	//Al menos de momento, activar las crisálidas aquí
	for (VEC3 spawnPos : spawnPositions) {
		TEntityParseContext ctx;
		ctx.root_transform.setPosition(spawnPos);
		parseScene(prefabToSpawn, ctx);
	}
	//EngineScripting.doFile(activationEffectFile);
}
