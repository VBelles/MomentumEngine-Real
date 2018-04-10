#include "mcv_platform.h"
#include "WallJumpSquatPlummetActionState.h"

WallJumpSquatPlummetActionState::WallJumpSquatPlummetActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
}

void WallJumpSquatPlummetActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	
	if (timer.elapsed() >= endingTime) {
		//saltar
		*velocityVector = GetPlayerTransform()->getFront() * frontVelocity;
		velocityVector->y = verticalVelocity;
		
		deltaMovement = *velocityVector * delta;
		GetPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::WallJumpPlummet);
	}
}

void WallJumpSquatPlummetActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	GetPlayerModel()->SetGravityMultiplier(0.f);
	GetPlayerModel()->maxVerticalSpeed = abs(verticalVelocity);
	SetPose();
	endingTime = endingFrames * (1.f / 60);
	timer.reset();
}

void WallJumpSquatPlummetActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
}
