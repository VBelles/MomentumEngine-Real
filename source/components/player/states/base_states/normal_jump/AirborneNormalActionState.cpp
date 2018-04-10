#include "mcv_platform.h"
#include "AirborneNormalActionState.h"
#include "components/comp_render.h"

AirborneNormalActionState::AirborneNormalActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
}

void AirborneNormalActionState::update (float delta) {
	AirborneActionState::update(delta);
}

void AirborneNormalActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	setPose();
}

void AirborneNormalActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
}

void AirborneNormalActionState::setPose() {
	getRender()->setMesh("data/meshes/pose_jump.mesh");
}

