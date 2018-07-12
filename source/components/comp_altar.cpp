#include "mcv_platform.h"
#include "comp_altar.h"
#include "components/comp_transform.h"
#include "components/player/attack_info.h"
#include "components/controllers/comp_rigid_anim.h"
#include "components/controllers/comp_rigid_anims_director.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("altar", TCompAltar);

void TCompAltar::debugInMenu() {
}

void TCompAltar::registerMsgs() {
    DECL_MSG(TCompAltar, TMsgAllScenesCreated, onAllScenesCreated);
	DECL_MSG(TCompAltar, TMsgDestroy, onDestroy);
	DECL_MSG(TCompAltar, TMsgAttackHit, onHit);
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
	if (j.count("prefab")) {
		prefabToSpawn = j["prefab"].get<std::string>();
	}
	else {
		dbg("altar: no sé qué tengo que spawnear\n");
	}
}

void TCompAltar::onAllScenesCreated(const TMsgAllScenesCreated & msg) {
}

void TCompAltar::onDestroy(const TMsgDestroy & msg) {
	EngineScripting.throwEvent(onAltarDestroyed, ((CEntity*)CHandle(this).getOwner())->getName());

	//Activar las crisálidas aquí, para poderlo integrar con 3dmax
	for (VEC3 spawnPos : spawnPositions) {
		TEntityParseContext ctx;
		ctx.root_transform.setPosition(spawnPos);
		parseScene(prefabToSpawn, ctx);
	}
}

void TCompAltar::onHit(const TMsgAttackHit & msg) {
	CEntity* entity = msg.attacker;
	if (entity->getName() == PLAYER_NAME && msg.info.damage > 0.f) {
		//llamar al que controle todas las animaciones y que él vaya entity a entity parándolas
		TCompRigidAnimsDirector* anims = get<TCompRigidAnimsDirector>();
		if (anims) {
			anims->setStopOnNextLoop(true);
		}
		EngineScripting.throwEvent(onAltarDestroyed, ((CEntity*)CHandle(this).getOwner())->getName());
		if (!isActivated) {
			isActivated = true;
			//Activar las crisálidas aquí, para poderlo integrar con 3dmax
			for (VEC3 spawnPos : spawnPositions) {
				TEntityParseContext ctx;
				ctx.root_transform.setPosition(spawnPos);
				parseScene(prefabToSpawn, ctx);
			}
		}
	}
}
