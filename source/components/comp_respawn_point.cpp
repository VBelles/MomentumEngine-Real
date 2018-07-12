#include "mcv_platform.h"
#include "comp_respawn_point.h"
#include "components/comp_collider.h"
#include "components/comp_transform.h"

DECL_OBJ_MANAGER("respawn_point", TCompRespawnPoint);

void TCompRespawnPoint::debugInMenu() {
}

void TCompRespawnPoint::registerMsgs() {
	DECL_MSG(TCompRespawnPoint, TMsgTriggerEnter, onTriggerEnter);
}

void TCompRespawnPoint::load(const json& j, TEntityParseContext& ctx) {
	if (j.count("respawn_position")) {
		respawnPosition = loadVEC3(j["respawn_position"]);
		if (j.count("respawn_rotation")) {
			CTransform transform = CTransform();
			transform.setRotation(loadQUAT(j["respawn_rotation"]));
			float pitch;
			transform.getYawPitchRoll(&respawnYaw, &pitch);
		}
		else {
			respawnYaw = j.value("respawn_yaw", respawnYaw);
		}
	}
}

void TCompRespawnPoint::onTriggerEnter(const TMsgTriggerEnter& msg) {
	CEntity* entity = msg.h_other_entity;
	entity->sendMsg(TMsgRespawnChanged{ respawnPosition , respawnYaw});
}
