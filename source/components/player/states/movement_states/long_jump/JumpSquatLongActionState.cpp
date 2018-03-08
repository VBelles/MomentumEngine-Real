#include "mcv_platform.h"
#include "JumpSquatLongActionState.h"

JumpSquatLongActionState::JumpSquatLongActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
}

void JumpSquatLongActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	PowerStats* currentPowerStats = GetPlayerModel()->GetPowerStats();
	
	if (timer.elapsed() >= squatTime) {
		//saltar
		GetPlayerModel()->isAttachedToPlatform = false;
		*velocityVector = GetPlayerTransform()->getFront() * currentPowerStats->longJumpVelocityVector.z;
		velocityVector->y = currentPowerStats->longJumpVelocityVector.y;
		deltaMovement = *velocityVector * delta;
	}
}

void JumpSquatLongActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	SetPose();
	//dbg("Entrando en JumpSquatLong\n");
	squatTime = squatFrames * (1.f / 60);
	timer.reset();
}

void JumpSquatLongActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	//dbg("Saliendo de JumpSquatLong\n");
}

//ni caso a este input
void JumpSquatLongActionState::OnJumpHighButton() {}

void JumpSquatLongActionState::OnJumpLongButton() {}

void JumpSquatLongActionState::OnLeavingGround() {
	if (timer.elapsed() >= squatTime) {
		timer.reset();
		GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::AirborneLong);
	}
	else {
		//En caso de que el comportamiento fuera diferente si cae antes de poder saltar
		GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::GhostJumpSquatLong);
	}
}
