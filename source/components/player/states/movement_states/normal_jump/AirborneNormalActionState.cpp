#include "mcv_platform.h"
#include "AirborneNormalActionState.h"

AirborneNormalActionState::AirborneNormalActionState(TCompPlayerModel * player)
	: AirborneActionState::AirborneActionState(player) {
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
	player->SetAttackState(TCompPlayerModel::ActionStates::GrabHigh);
}

void AirborneNormalActionState::OnLanding() {
	//Ir a landing action state
	player->SetMovementState(TCompPlayerModel::ActionStates::Run);
}
