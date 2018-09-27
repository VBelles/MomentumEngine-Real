#include "mcv_platform.h"
#include "WallJumpSquatPlummetActionState.h"

WallJumpSquatPlummetActionState::WallJumpSquatPlummetActionState(StateManager* stateManager) :
	AirborneActionState(stateManager, WallJumpSquatPlummet) {
}

void WallJumpSquatPlummetActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	if (!stateManager->isChangingBaseState) {
		if (timer.elapsed() >= endingTime) {
			//saltar
			*velocityVector = getPlayerTransform()->getFront() * frontVelocity;
			velocityVector->y = verticalVelocity;

			deltaMovement = *velocityVector * delta;
			getSkeleton()->executeAction(animation, 0.1f, 0.1f);
			stateManager->changeState(WallJumpPlummet);
		}
	}
}

void WallJumpSquatPlummetActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	getPlayerModel()->setGravityMultiplier(0.f);
	getPlayerModel()->maxVerticalSpeed = abs(verticalVelocity);
	getSkeleton()->blendCycle(animationHugging, 0.1f, 0.1f);
	timer.reset();
	EngineSound.emitEvent(SOUND_JUMP_LONG);
}

void WallJumpSquatPlummetActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
}
