#include "mcv_platform.h"
#include "HuggingWallJumpSquatActionState.h"

HuggingWallJumpSquatActionState::HuggingWallJumpSquatActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
}

void HuggingWallJumpSquatActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	//deltaMovement.y = velocityVector->y * delta;
	PowerStats* currentPowerStats = GetPlayerModel()->GetPowerStats();
	
	if (timer.elapsed() >= endingTime) {
		//saltar
		*velocityVector = GetPlayerTransform()->getFront() * currentPowerStats->wallJumpVelocityVector.z;
		velocityVector->y = currentPowerStats->wallJumpVelocityVector.y;
		
		deltaMovement = *velocityVector * delta;
		GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::AirborneWallJump);
	}
}

void HuggingWallJumpSquatActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	SetPose();
	endingTime = endingFrames * (1.f / 60);
	timer.reset();
}

void HuggingWallJumpSquatActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
}