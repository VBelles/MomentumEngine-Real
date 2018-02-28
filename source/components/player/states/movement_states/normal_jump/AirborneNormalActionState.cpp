#include "mcv_platform.h"
#include "AirborneNormalActionState.h"

AirborneNormalActionState::AirborneNormalActionState(CHandle playerHandle)
	: AirborneActionState::AirborneActionState(playerHandle) {
}

void AirborneNormalActionState::update (float delta) {
	AirborneActionState::update(delta);
}

void AirborneNormalActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
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
	GetPlayer()->SetAttackState(TCompPlayerModel::ActionStates::GrabHigh);
}

void AirborneNormalActionState::OnJumpLongButton() {
	GetPlayer()->SetAttackState(TCompPlayerModel::ActionStates::GrabLong);
}

void AirborneNormalActionState::OnLanding() {
	//Ir a landing action state
	GetPlayer()->SetMovementState(TCompPlayerModel::ActionStates::Run);
}
