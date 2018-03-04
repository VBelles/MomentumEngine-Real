#include "mcv_platform.h"
#include "RunActionState.h"

RunActionState::RunActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
}

void RunActionState::update (float delta) {
	GroundedActionState::update(delta);
	if (velocityVector->x == 0.f && velocityVector->z == 0.f) {
		GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::Idle);
	}
}

void RunActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	SetPose();
	dbg("Entrando en run\n");
}

void RunActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	dbg("Saliendo de run\n");
}

void RunActionState::SetMovementInput(VEC2 input) {
	movementInput = input;
}

void RunActionState::OnJumpHighButton() {
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::JumpSquat);
}

void RunActionState::OnJumpLongButton() {
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::JumpSquatLong);
}

void RunActionState::OnLeavingGround() {
	//Set state a alguno por defecto, luego las clases derivadas de esta ya sabrán qué hacer
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}
