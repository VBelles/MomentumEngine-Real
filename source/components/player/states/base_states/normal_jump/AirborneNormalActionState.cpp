#include "mcv_platform.h"
#include "AirborneNormalActionState.h"
#include "components/comp_render.h"
#include "components/player/comp_player_model.h"
#include "skeleton/comp_skeleton.h"

AirborneNormalActionState::AirborneNormalActionState(StateManager* stateManager) :
	AirborneActionState(stateManager, AirborneNormal) {
}

void AirborneNormalActionState::update(float delta) {
	AirborneActionState::update(delta);
}

void AirborneNormalActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	getSkeleton()->blendCycle(animationFalling, 0.1f, 0.1f);

}

void AirborneNormalActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
}

