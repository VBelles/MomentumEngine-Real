#include "mcv_platform.h"
#include "AirborneNormalActionState.h"

AirborneNormalActionState::AirborneNormalActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
}

void AirborneNormalActionState::update (float delta) {
	AirborneActionState::update(delta);
}

void AirborneNormalActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	GetRender()->setMesh("data/meshes/pose_jump.mesh");
	//dbg("Entrando en airborne normal\n");
}

void AirborneNormalActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	//dbg("Saliendo de airborne normal\n");
}

void AirborneNormalActionState::SetMovementInput(VEC2 input) {
	movementInput = input;
}

void AirborneNormalActionState::OnJumpHighButton() {
	GetPlayerModel()->SetAttackState(TCompPlayerModel::ActionStates::GrabHigh);
}

void AirborneNormalActionState::OnJumpLongButton() {
	GetPlayerModel()->SetAttackState(TCompPlayerModel::ActionStates::GrabLong);
}

void AirborneNormalActionState::OnLanding() {
	//Ir a landing action state
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::Landing);
}
