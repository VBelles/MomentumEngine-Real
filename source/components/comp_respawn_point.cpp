#include "mcv_platform.h"
#include "comp_respawn_point.h"
#include "components/comp_collider.h"

DECL_OBJ_MANAGER("respawn_point", TCompRespawnPoint);

void TCompRespawnPoint::debugInMenu() {
}

void TCompRespawnPoint::registerMsgs() {
	DECL_MSG(TCompRespawnPoint, TMsgTriggerEnter, onTriggerEnter);
}

void TCompRespawnPoint::load(const json& j, TEntityParseContext& ctx) {
	if (j.count("respawn_position")) {
		respawnPosition = loadVEC3(j["respawn_position"]);
	}
}

void TCompRespawnPoint::onTriggerEnter(const TMsgTriggerEnter& msg) {
	CEntity* entity = msg.h_other_entity;
	entity->sendMsg(TMsgRespawnChanged{ respawnPosition });
}
