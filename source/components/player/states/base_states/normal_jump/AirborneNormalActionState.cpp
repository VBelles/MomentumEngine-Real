#include "mcv_platform.h"
#include "AirborneNormalActionState.h"

AirborneNormalActionState::AirborneNormalActionState(StateManager* stateManager) :
	AirborneActionState(stateManager, AirborneNormal) {
}

void AirborneNormalActionState::update(float delta) {
	AirborneActionState::update(delta);
}

void AirborneNormalActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	getSkeleton()->blendCycle(animation, 0.3f, 0.3f);

}

void AirborneNormalActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
}

