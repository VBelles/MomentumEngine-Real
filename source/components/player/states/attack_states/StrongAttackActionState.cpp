#include "mcv_platform.h"
#include "StrongAttackActionState.h"

StrongAttackActionState::StrongAttackActionState(TCompPlayerModel * player)
	: GroundedActionState::GroundedActionState(player) {
}

void StrongAttackActionState::update (float delta) {
	//Ataque en sí
	dbg("Strong Attack\n");
}

void StrongAttackActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
}

void StrongAttackActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
}

void StrongAttackActionState::SetMovementInput(VEC2 input) {
}

void StrongAttackActionState::OnJumpHighButton() {
}

void StrongAttackActionState::OnJumpLongButton() {
}

void StrongAttackActionState::OnLeavingGround() {
	player->SetMovementState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}
