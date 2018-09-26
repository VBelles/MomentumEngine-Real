#include "mcv_platform.h"
#include "scripting_player.h"
#include "components/player/comp_player_model.h"
#include "components/player/comp_player_controller.h"
#include "components/player/comp_power_gauge.h"
#include "components/controllers/comp_camera_player.h"
#include "components/player/states/StateManager.h"
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
	manager->set("stopPlayerVelocity", SLB::FuncCall::create(ScriptingPlayer::stopPlayerVelocity));
	manager->set("changePlayerState", SLB::FuncCall::create(ScriptingPlayer::changePlayerState));
	manager->set("changePlayerConcurrentState", SLB::FuncCall::create(ScriptingPlayer::changePlayerConcurrentState));
	manager->set("walkTo", SLB::FuncCall::create(ScriptingPlayer::walkTo));
	manager->set("rotatePlayerTowards", SLB::FuncCall::create(ScriptingPlayer::rotatePlayerTowards));
	manager->set("lockPlayerCameraInput", SLB::FuncCall::create(ScriptingPlayer::lockPlayerCameraInput));
	manager->set("unlockPlayerCameraInput", SLB::FuncCall::create(ScriptingPlayer::unlockPlayerCameraInput));
	manager->set("disablePlayerOutline", SLB::FuncCall::create(ScriptingPlayer::disablePlayerOutline));
	manager->set("enablePlayerOutline", SLB::FuncCall::create(ScriptingPlayer::enablePlayerOutline));
	manager->set("isPlayerGrounded", SLB::FuncCall::create(ScriptingPlayer::isPlayerGrounded));
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
	bool ret = InputType::Menu != Engine.getInputType();
	Engine.setInputType(InputType::Menu);
	return ret;
}

bool ScriptingPlayer::givePlayerControl() {
	bool ret = InputType::Game != Engine.getInputType();
	Engine.setInputType(InputType::Game);
	return ret;
}

void ScriptingPlayer::lockPlayerCameraInput() {
	getPlayerCamera()->lockCameraInput(true);
}

void ScriptingPlayer::unlockPlayerCameraInput() {
	getPlayerCamera()->lockCameraInput(false);
}

void ScriptingPlayer::disablePlayerOutline() {
	getPlayerModel()->disableOutline();
}

void ScriptingPlayer::enablePlayerOutline() {
	getPlayerModel()->enableOutline();
}

void ScriptingPlayer::teleportPlayer(float x, float y, float z) {
	getCollider()->controller->setFootPosition(physx::PxExtendedVec3(x, y, z));
}

void ScriptingPlayer::movePlayer(float dX, float dY, float dZ) {
	physx::PxExtendedVec3 position = getCollider()->controller->getFootPosition();
	getCollider()->controller->setFootPosition(position + physx::PxExtendedVec3(dX, dY, dZ));
}

void ScriptingPlayer::setPower(float power) {
	getPowerGauge()->setPower(power);
}

void ScriptingPlayer::stopPlayerVelocity() {
	getPlayerModel()->stopPlayerVelocity();
}

void ScriptingPlayer::changePlayerState(std::string state) {
	getPlayerModel()->changeState(state);
}

void ScriptingPlayer::changePlayerConcurrentState(std::string state) {
	getPlayerModel()->changeConcurrentState(state);
}

void ScriptingPlayer::walkTo(float x, float z) {
	VEC3 targetPosition = VEC3(x, 0, z);
	getPlayerModel()->walkTo(targetPosition);
}

void ScriptingPlayer::rotatePlayerTowards(float x, float z, float speed) {
	VEC3 targetPosition = VEC3(x, 0, z);
	getPlayerModel()->rotatePlayerTowards(targetPosition, speed);
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

TCompCameraPlayer* ScriptingPlayer::getPlayerCamera() {
	CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
	return playerCameraEntity->get<TCompCameraPlayer>();
}

bool ScriptingPlayer::isPlayerGrounded() {
	return getPlayerModel()->isGrounded();
}
