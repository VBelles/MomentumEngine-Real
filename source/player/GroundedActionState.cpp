#include "mcv_platform.h"
#include "GroundedActionState.h"

GroundedActionState::GroundedActionState(TCompPlayerModel * player)
	: IActionState::IActionState(player) {
}

void GroundedActionState::update (float delta) {
}

void GroundedActionState::OnStateEnter(IActionState * lastState) {
	IActionState::OnStateEnter(lastState);
	dbg("entrando en grounded\n");
}

void GroundedActionState::OnStateExit(IActionState * nextState) {
	IActionState::OnStateExit(nextState);
}

void GroundedActionState::OnJumpHighButton() {
	//saltar
}

void GroundedActionState::OnLeavingGround() {
	//Set state a alguno por defecto, luego las clases derivadas de esta ya sabrán qué hacer
	player->SetActionState(TCompPlayerModel::ActionStates::Airborne);
}
