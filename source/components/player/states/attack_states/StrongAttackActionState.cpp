#include "mcv_platform.h"
#include "StrongAttackActionState.h"
#include "components/comp_hitbox.h"

StrongAttackActionState::StrongAttackActionState(TCompPlayerModel * player, CHandle hitbox)
	: GroundedActionState::GroundedActionState(player) {
	hitboxHandle = hitbox;
}

void StrongAttackActionState::update (float delta) {
	if (timer.elapsed() > animationEndTime) {
		player->SetAttackState(TCompPlayerModel::ActionStates::Idle);
		player->lockMovementState = false;
		player->lockWalk = false;
	}
	else if (timer.elapsed() > hitEndTime) {
		CEntity *hitboxEntity = hitboxHandle;
		TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
		hitbox->disable();
	}
	else if (timer.elapsed() > hitboxOutTime) {
		CEntity *hitboxEntity = hitboxHandle;
		TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
		hitbox->enable();
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

void StrongAttackActionState::OnHitboxEnter(CHandle entity) {
	if (entity != CHandle(player).getOwner()) {
		CEntity *otherEntity = entity;
		otherEntity->sendMsg(TMsgAttackHit{ entity, damage });
		dbg("Strong attack hit for %i damage\n", damage);
	}
}