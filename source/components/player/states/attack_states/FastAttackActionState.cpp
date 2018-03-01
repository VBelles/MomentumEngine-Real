#include "mcv_platform.h"
#include "FastAttackActionState.h"
#include "components/comp_hitbox.h"

FastAttackActionState::FastAttackActionState(CHandle playerModelHandle, CHandle hitbox)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
	hitboxHandle = hitbox;
}

void FastAttackActionState::update (float delta) {
	if (isLauncher && timer.elapsed() > beginLauncherTime) {
		dbg("Cambio de strong attack a vertical launcher\n");
		GetPlayerModel()->SetAttackState(TCompPlayerModel::ActionStates::HorizontalLauncher);
	}
	else {
		if (timer.elapsed() > animationEndTime) {
			GetPlayerModel()->SetAttackState(TCompPlayerModel::ActionStates::Idle);
			GetPlayerModel()->lockMovementState = false;
			GetPlayerModel()->lockWalk = false;
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
}

void FastAttackActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	dbg("Strong Attack\n");
	hitboxOutTime = warmUpFrames * (1.f / 60);
	hitEndTime = hitboxOutTime + activeFrames * (1.f / 60);
	animationEndTime = hitEndTime + endingLagFrames * (1.f / 60);
	interruptibleTime = IASAFrames * (1.f / 60);
	beginLauncherTime = startLauncherFrames * (1.f / 60);
	isLauncher = true;
	timer.reset();
	GetPlayerModel()->lockMovementState = true;
	GetPlayerModel()->lockWalk = true;
}

void FastAttackActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	dbg("Finish strong Attack\n");
}

void FastAttackActionState::SetMovementInput(VEC2 input) {
}

void FastAttackActionState::OnJumpHighButton() {
}

void FastAttackActionState::OnJumpLongButton() {
}

void FastAttackActionState::OnFastAttackButtonReleased() {
	isLauncher = false;
}

void FastAttackActionState::OnLeavingGround() {
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}

void FastAttackActionState::OnHitboxEnter(CHandle entity) {
	CHandle playerEntity = playerModelHandle.getOwner();
	if (entity != playerEntity) {
		CEntity *otherEntity = entity;
		otherEntity->sendMsg(TMsgAttackHit{ playerEntity, damage });
		dbg("Strong attack hit for %i damage\n", damage);
	}
}