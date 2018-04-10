#include "mcv_platform.h"
#include "WallJumpSquatActionState.h"

WallJumpSquatActionState::WallJumpSquatActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
}

void WallJumpSquatActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	//deltaMovement.y = velocityVector->y * delta;
	PowerStats* currentPowerStats = GetPlayerModel()->GetPowerStats();
	
	if (timer.elapsed() >= endingTime) {
		//saltar
		*velocityVector = GetPlayerTransform()->getFront() * currentPowerStats->wallJumpVelocityVector.z;
		velocityVector->y = currentPowerStats->wallJumpVelocityVector.y;
		
		deltaMovement = *velocityVector * delta;
		GetPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneWallJump);
	}
}

void WallJumpSquatActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	SetPose();
	endingTime = endingFrames * (1.f / 60);
	timer.reset();
}

void WallJumpSquatActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
}