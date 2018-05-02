#include "mcv_platform.h"
#include "DeathActionState.h"
#include "components/comp_render.h"
#include "components/player/comp_player_model.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/comp_power_gauge.h"

DeathActionState::DeathActionState(CHandle playerModelHandle)
	: IActionState::IActionState(playerModelHandle) {
	animation = "walk";
}

void DeathActionState::update (float delta) {
	if (finish) {
		respawn();
	}
}

void DeathActionState::onStateEnter(IActionState* lastState) {
	IActionState::onStateEnter(lastState);
	getPlayerModel()->getSkeleton()->blendCycle(animation);
	finish = true;
	Engine.getScripting().throwEvent(onPlayerKilled, "");
}

void DeathActionState::onStateExit(IActionState* nextState) {
	IActionState::onStateExit(nextState);
}

void DeathActionState::respawn() {
	*velocityVector = VEC3();
	assert(getPlayerModel());
	VEC3 respawnPosition = getPlayerModel()->getRespawnPosition();
	getCollider()->controller->setFootPosition(PxExtendedVec3(respawnPosition.x, respawnPosition.y, respawnPosition.z));
	getPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::Idle);
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	getPlayerModel()->resetHp();
	getPlayerModel()->getPowerGauge()->resetPower();
	CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
	Engine.getCameras().blendInCamera(playerCameraEntity, 0.001f, CModuleCameras::EPriority::GAMEPLAY);
}


