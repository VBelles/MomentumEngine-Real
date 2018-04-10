#include "mcv_platform.h"
#include "WallJumpSquatPlummetActionState.h"

WallJumpSquatPlummetActionState::WallJumpSquatPlummetActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
}

void WallJumpSquatPlummetActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	
	if (timer.elapsed() >= endingTime) {
		//saltar
		*velocityVector = getPlayerTransform()->getFront() * frontVelocity;
		velocityVector->y = verticalVelocity;
		
		deltaMovement = *velocityVector * delta;
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::WallJumpPlummet);
	}
}

void WallJumpSquatPlummetActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	getPlayerModel()->setGravityMultiplier(0.f);
	getPlayerModel()->maxVerticalSpeed = abs(verticalVelocity);
	setPose();
	endingTime = endingFrames * (1.f / 60);
	timer.reset();
}

void WallJumpSquatPlummetActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
}
