#include "mcv_platform.h"
#include "VerticalLauncherActionState.h"
#include "components/comp_hitbox.h"

VerticalLauncherActionState::VerticalLauncherActionState(TCompPlayerModel * player, CHandle hitbox)
	: GroundedActionState::GroundedActionState(player) {
	hitboxHandle = hitbox;
}

void VerticalLauncherActionState::update (float delta) {
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

void VerticalLauncherActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	dbg("Vertical Launcher\n");
	hitboxOutTime = warmUpFrames * (1.f / 60);
	hitEndTime = hitboxOutTime + activeFrames * (1.f / 60);
	animationEndTime = hitEndTime + endingLagFrames * (1.f / 60);
	interruptibleTime = IASAFrames * (1.f / 60);
	timer.reset();
	player->lockMovementState = true;
	player->lockWalk = true;
}

void VerticalLauncherActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	dbg("Finish Vertical Launcher\n");
}

void VerticalLauncherActionState::SetMovementInput(VEC2 input) {
}

void VerticalLauncherActionState::OnJumpHighButton() {
}

void VerticalLauncherActionState::OnJumpLongButton() {
}

void VerticalLauncherActionState::OnLeavingGround() {
	player->SetMovementState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}

void VerticalLauncherActionState::OnHitboxEnter(CHandle entity) {
	CHandle playerHandle = CHandle(player).getOwner();
	if (entity != playerHandle) {
		CEntity *otherEntity = entity;
		otherEntity->sendMsg(TMsgLaunchedVertically{ playerHandle, damage });
		dbg("Vertical Launcher hit for %i damage\n", damage);
	}
}