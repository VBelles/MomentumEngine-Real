#include "mcv_platform.h"
#include "RunActionState.h"

RunActionState::RunActionState(CHandle playerHandle)
	: GroundedActionState::GroundedActionState(playerHandle) {
}

void RunActionState::update (float delta) {
	GroundedActionState::update(delta);
}

void RunActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	//dbg("Entrando en run\n");
}

void RunActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	//dbg("Saliendo de run\n");
}

void RunActionState::SetMovementInput(VEC2 input) {
	movementInput = input;
}

void RunActionState::OnJumpHighButton() {
	GetPlayer()->SetMovementState(TCompPlayerModel::ActionStates::JumpSquat);
}

void RunActionState::OnJumpLongButton() {
	GetPlayer()->SetMovementState(TCompPlayerModel::ActionStates::JumpSquatLong);
}

void RunActionState::OnLeavingGround() {
	//Set state a alguno por defecto, luego las clases derivadas de esta ya sabrán qué hacer
	GetPlayer()->SetMovementState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}
