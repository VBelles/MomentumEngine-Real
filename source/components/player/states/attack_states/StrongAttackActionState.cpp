#include "mcv_platform.h"
#include "StrongAttackActionState.h"

StrongAttackActionState::StrongAttackActionState(TCompPlayerModel * player)
	: GroundedActionState::GroundedActionState(player) {
}

void StrongAttackActionState::update (float delta) {
	//Ataque en sí
	if (timer.elapsed() > animationEndTime) {
		player->SetAttackState(TCompPlayerModel::ActionStates::Idle);
		player->lockMovementState = false;
		player->lockWalk = false;
	}
}

void StrongAttackActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	dbg("Strong Attack\n");
	hitboxOutTime = warmUpFrames * (1.f / 60);
	hitEndTime = hitboxOutTime + activeFrames * (1.f / 60);
	animationEndTime = hitEndTime + endingLagFrames * (1.f / 60);
	interruptibleTime = IASAFrames * (1.f / 60);
	timer.reset();
	player->lockMovementState = true;
	player->lockWalk = true;
}

void StrongAttackActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	dbg("Finish strong Attack\n");
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
