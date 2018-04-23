#include "mcv_platform.h"
#include "PitFallingActionState.h"
#include "components/comp_render.h"
#include "components/player/comp_player_model.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/comp_power_gauge.h"

PitFallingActionState::PitFallingActionState(CHandle playerModelHandle)
	: IActionState::IActionState(playerModelHandle) {
	animation = "walk";
}

void PitFallingActionState::update (float delta) {
	if (finish) {
		respawn();
	}
}

void PitFallingActionState::onStateEnter(IActionState* lastState) {
	IActionState::onStateEnter(lastState);
	getPlayerModel()->getSkeleton()->blendCycle(animation);
	finish = true;
}

void PitFallingActionState::onStateExit(IActionState* nextState) {
	IActionState::onStateExit(nextState);
}

void PitFallingActionState::respawn() {
	*velocityVector = VEC3();
	assert(getPlayerModel());
	VEC3 respawnPosition = getPlayerModel()->getRespawnPosition();
	getCollider()->controller->setFootPosition(PxExtendedVec3(respawnPosition.x, respawnPosition.y, respawnPosition.z));
	getPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::Idle);
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Idle);
	getPlayerModel()->damage(fallingDamage);
	CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
	Engine.getCameras().blendInCamera(playerCameraEntity, 0.001f, CModuleCameras::EPriority::GAMEPLAY);
}


