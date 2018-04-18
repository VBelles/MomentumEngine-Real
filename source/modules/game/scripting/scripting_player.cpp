#include "mcv_platform.h"
#include "scripting_player.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "components/player/comp_player_model.h"
#include "components/player/comp_power_gauge.h"

ScriptingPlayer::ScriptingPlayer() {
	CEntity* playerEntity = getEntityByName(PLAYER_NAME);
	assert(playerEntity);
	playerModelHandle = playerEntity->get<TCompPlayerModel>();
	transformHandle = playerEntity->get<TCompTransform>();
	colliderHandle = playerEntity->get<TCompCollider>();
	powerGaugeHandle = playerEntity->get<TCompPowerGauge>();
	assert(playerModelHandle.isValid());
	assert(transformHandle.isValid());
	assert(colliderHandle.isValid());

}

ScriptingPlayer::~ScriptingPlayer() {
}


float ScriptingPlayer::getHp() {
	return getPlayerModel()->getHp();
}

void ScriptingPlayer::teleport(float x, float y, float z) {
	getCollider()->controller->setFootPosition(physx::PxExtendedVec3(x, y, z));
}

void ScriptingPlayer::move(float dX, float dY, float dZ){
	physx::PxExtendedVec3 position = getCollider()->controller->getFootPosition();
	getCollider()->controller->setFootPosition(position + physx::PxExtendedVec3(dX, dY, dZ));
}

void ScriptingPlayer::setPower(float power) {
	getPowerGauge()->setPower(power);
}

TCompPlayerModel* ScriptingPlayer::getPlayerModel() {
	return playerModelHandle;
}

TCompTransform * ScriptingPlayer::getTransform() {
	return transformHandle;
}

TCompCollider * ScriptingPlayer::getCollider() {
	return colliderHandle;
}

TCompPowerGauge* ScriptingPlayer::getPowerGauge() {
	return powerGaugeHandle;
}





