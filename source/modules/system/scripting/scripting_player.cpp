#include "mcv_platform.h"
#include "scripting_player.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "components/player/comp_player_model.h"
#include "components/player/comp_player_controller.h"
#include "components/player/comp_power_gauge.h"
#include <SLB/SLB.hpp>

ScriptingPlayer::ScriptingPlayer() {
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
	CEntity* playerEntity = getEntityByName(PLAYER_NAME);
	playerModelHandle = playerEntity->get<TCompPlayerModel>();
	return playerModelHandle;
}

TCompPlayerController * ScriptingPlayer::getPlayerController() {
	CEntity* playerEntity = getEntityByName(PLAYER_NAME);
	playerControllerHandle = playerEntity->get<TCompPlayerController>();
	return playerControllerHandle;
}

TCompTransform * ScriptingPlayer::getTransform() {
	CEntity* playerEntity = getEntityByName(PLAYER_NAME);
	transformHandle = playerEntity->get<TCompTransform>();
	return transformHandle;
}

TCompCollider * ScriptingPlayer::getCollider() {
	CEntity* playerEntity = getEntityByName(PLAYER_NAME);
	colliderHandle = playerEntity->get<TCompCollider>();
	return colliderHandle;
}

TCompPowerGauge* ScriptingPlayer::getPowerGauge() {
	CEntity* playerEntity = getEntityByName(PLAYER_NAME);
	powerGaugeHandle = playerEntity->get<TCompPowerGauge>();
	return powerGaugeHandle;
}