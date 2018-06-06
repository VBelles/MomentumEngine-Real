#include "mcv_platform.h"
#include "DeathActionState.h"
#include "components/comp_render.h"
#include "components/player/comp_player_model.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/comp_power_gauge.h"
#include "components/player/states/StateManager.h"

DeathActionState::DeathActionState(StateManager* stateManager) :
	IActionState(stateManager, Death) {
}

void DeathActionState::update(float delta) {
	if (finish) {
		respawn();
	}
}

void DeathActionState::onStateEnter(IActionState* lastState) {
	IActionState::onStateEnter(lastState);
	getSkeleton()->blendCycle(animation);
	finish = true;
	EngineScripting.throwEvent(onPlayerKilled, "");
}

void DeathActionState::onStateExit(IActionState* nextState) {
	IActionState::onStateExit(nextState);
}

void DeathActionState::respawn() {
	*velocityVector = VEC3();
	assert(getPlayerModel());
	VEC3 respawnPosition = getPlayerModel()->getRespawnPosition();
	getCollider()->controller->setFootPosition(PxExtendedVec3(respawnPosition.x, respawnPosition.y, respawnPosition.z));
	stateManager->changeConcurrentState(Free);
	stateManager->changeState(AirborneNormal);
	getPlayerModel()->resetHp();
	getPowerGauge()->resetPower();
	CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
	EngineCameras.blendInCamera(playerCameraEntity, 0.001f, CModuleCameras::EPriority::GAMEPLAY);
}


