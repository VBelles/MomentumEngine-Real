#include "mcv_platform.h"
#include "AirborneNormalActionState.h"
#include "components/comp_render.h"
#include "components/player/comp_player_model.h"
#include "skeleton/comp_skeleton.h"

AirborneNormalActionState::AirborneNormalActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
	animation = "walk";
}

void AirborneNormalActionState::update (float delta) {
	AirborneActionState::update(delta);
}

void AirborneNormalActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	setPose();
	getPlayerModel()->getSkeleton()->blendCycle(animation);
}

void AirborneNormalActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
}

void AirborneNormalActionState::setPose() {
	getRender()->setMesh("data/meshes/pose_jump.mesh");
}

