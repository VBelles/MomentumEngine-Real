#include "mcv_platform.h"
#include "scripting_player.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "components/player/comp_player_model.h"


ScriptingPlayer::ScriptingPlayer() {
	CEntity* playerEntity = getEntityByName(PLAYER_NAME);
	playerModelHandle = playerEntity->get<TCompPlayerModel>();
	transformHandle = playerEntity->get<TCompTransform>();
	colliderHandle = playerEntity->get<TCompCollider>();
}

ScriptingPlayer::~ScriptingPlayer() {
}


float ScriptingPlayer::getHp() {
	return getPlayerModel()->getHp();
}

void ScriptingPlayer::teleport(float x, float y, float z) {
	getCollider()->controller->setFootPosition(physx::PxExtendedVec3(x, y, z));
}

TCompPlayerModel* ScriptingPlayer::getPlayerModel() {
	return playerModelHandle;
}

TCompTransform * ScriptingPlayer::getTransform() {
	return nullptr;
}

TCompCollider * ScriptingPlayer::getCollider() {
	return nullptr;
}


