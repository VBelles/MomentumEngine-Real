#include "mcv_platform.h"
#include "VerticalLauncherActionState.h"
#include "components/comp_hitbox.h"

VerticalLauncherActionState::VerticalLauncherActionState(CHandle playerHandle, CHandle hitbox)
	: GroundedActionState::GroundedActionState(playerHandle) {
	hitboxHandle = hitbox;
}

void VerticalLauncherActionState::update (float delta) {
	if (timer.elapsed() > animationEndTime) {
		GetPlayer()->SetAttackState(TCompPlayerModel::ActionStates::Idle);
		GetPlayer()->lockMovementState = false;
		GetPlayer()->lockWalk = false;
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
	GetPlayer()->lockMovementState = true;
	GetPlayer()->lockWalk = true;
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
	GetPlayer()->SetMovementState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}

void VerticalLauncherActionState::OnHitboxEnter(CHandle entity) {
	CHandle playerEntity = playerHandle.getOwner();
	if (entity != playerEntity) {
		CEntity *otherEntity = entity;
		otherEntity->sendMsg(TMsgAttackHit{ playerEntity, damage });
		otherEntity->sendMsg(TMsgLaunchedVertically{ playerEntity, damage });
		dbg("Vertical Launcher hit for %i damage\n", damage);
	}
}