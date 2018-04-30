#include "mcv_platform.h"
#include "scripting_player.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "components/player/comp_player_model.h"
#include "components/player/comp_player_controller.h"
#include "components/player/comp_power_gauge.h"
#include <SLB/SLB.hpp>

void ScriptingPlayer::bind(SLB::Manager* manager) {
	manager->set("getPlayerHp", SLB::FuncCall::create(ScriptingPlayer::getPlayerHp));
	manager->set("setPlayerHp", SLB::FuncCall::create(ScriptingPlayer::setPlayerHp));
	manager->set("setPlayerRespawnPosition", SLB::FuncCall::create(ScriptingPlayer::setPlayerRespawnPosition));
	manager->set("takePlayerControl", SLB::FuncCall::create(ScriptingPlayer::takePlayerControl));
	manager->set("givePlayerControl", SLB::FuncCall::create(ScriptingPlayer::givePlayerControl));
	manager->set("teleportPlayer", SLB::FuncCall::create(ScriptingPlayer::teleportPlayer));
	manager->set("movePlayer", SLB::FuncCall::create(ScriptingPlayer::movePlayer));
	manager->set("setPower", SLB::FuncCall::create(ScriptingPlayer::setPower));
}


float ScriptingPlayer::getPlayerHp() {
	return getPlayerModel()->getHp();
}

float ScriptingPlayer::setPlayerHp(float hp) {
	getPlayerModel()->setHp(hp);
	return getPlayerModel()->getHp();
}

void ScriptingPlayer::setPlayerRespawnPosition(float x, float y, float z) {
	VEC3 position = VEC3(x, y, z);
	getPlayerModel()->setRespawnPosition(position);
}

bool ScriptingPlayer::takePlayerControl() {
	return getPlayerController()->takePlayerControl();
}

bool ScriptingPlayer::givePlayerControl() {
	return getPlayerController()->givePlayerControl();
}

void ScriptingPlayer::teleportPlayer(float x, float y, float z) {
	getCollider()->controller->setFootPosition(physx::PxExtendedVec3(x, y, z));
}

void ScriptingPlayer::movePlayer(float dX, float dY, float dZ){
	physx::PxExtendedVec3 position = getCollider()->controller->getFootPosition();
	getCollider()->controller->setFootPosition(position + physx::PxExtendedVec3(dX, dY, dZ));
}

void ScriptingPlayer::setPower(float power) {
	getPowerGauge()->setPower(power);
}


TCompPlayerModel* ScriptingPlayer::getPlayerModel() {
	CEntity* playerEntity = getEntityByName(PLAYER_NAME);
	return  playerEntity->get<TCompPlayerModel>();
}

TCompPlayerController * ScriptingPlayer::getPlayerController() {
	CEntity* playerEntity = getEntityByName(PLAYER_NAME);
	return playerEntity->get<TCompPlayerController>();
}

TCompTransform * ScriptingPlayer::getTransform() {
	CEntity* playerEntity = getEntityByName(PLAYER_NAME);
	return playerEntity->get<TCompTransform>();
}

TCompCollider * ScriptingPlayer::getCollider() {
	CEntity* playerEntity = getEntityByName(PLAYER_NAME);
	return playerEntity->get<TCompCollider>();
}

TCompPowerGauge* ScriptingPlayer::getPowerGauge() {
	CEntity* playerEntity = getEntityByName(PLAYER_NAME);
	return playerEntity->get<TCompPowerGauge>();
}