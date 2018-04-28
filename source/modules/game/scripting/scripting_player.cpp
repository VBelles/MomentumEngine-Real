#include "mcv_platform.h"
#include "scripting_player.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "components/player/comp_player_model.h"
#include "components/player/comp_player_controller.h"
#include "components/player/comp_power_gauge.h"
#include <SLB/SLB.hpp>

ScriptingPlayer::ScriptingPlayer() {
	CEntity* playerEntity = getEntityByName(PLAYER_NAME);
	assert(playerEntity);
	playerModelHandle = playerEntity->get<TCompPlayerModel>();
	transformHandle = playerEntity->get<TCompTransform>();
	colliderHandle = playerEntity->get<TCompCollider>();
	powerGaugeHandle = playerEntity->get<TCompPowerGauge>();
	playerControllerHandle = playerEntity->get<TCompPlayerController>();
	assert(playerModelHandle.isValid());
	assert(transformHandle.isValid());
	assert(colliderHandle.isValid());
	assert(playerControllerHandle.isValid());

}

ScriptingPlayer::~ScriptingPlayer() {
}


void ScriptingPlayer::bind(SLB::Manager* manager) {
	SLB::Class<ScriptingPlayer>("Player", manager)
		.constructor()
		.set("getHp", &ScriptingPlayer::getHp)
		.set("setHp", &ScriptingPlayer::setHp)
		.set("teleport", &ScriptingPlayer::teleport)
		.set("tp", &ScriptingPlayer::teleport)
		.set("move", &ScriptingPlayer::move)
		.set("setPower", &ScriptingPlayer::setPower)
		.set("setRespawnPosition", &ScriptingPlayer::setRespawnPosition)
		.set("takeControl", &ScriptingPlayer::takePlayerControl)
		.set("giveControl", &ScriptingPlayer::givePlayerControl);
}


float ScriptingPlayer::getHp() {
	return getPlayerModel()->getHp();
}

float ScriptingPlayer::setHp(float hp) {
	getPlayerModel()->setHp(hp);
	return getPlayerModel()->getHp();
}

void ScriptingPlayer::setRespawnPosition(float x, float y, float z) {
	VEC3 position = VEC3(x, y, z);
	getPlayerModel()->setRespawnPosition(position);
}

bool ScriptingPlayer::takePlayerControl() {
	return getPlayerController()->takePlayerControl();
}

bool ScriptingPlayer::givePlayerControl() {
	return getPlayerController()->givePlayerControl();
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

TCompPlayerController * ScriptingPlayer::getPlayerController() {
	return playerControllerHandle;
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