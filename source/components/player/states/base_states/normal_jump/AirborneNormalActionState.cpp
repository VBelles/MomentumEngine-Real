#include "mcv_platform.h"
#include "AirborneNormalActionState.h"

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

