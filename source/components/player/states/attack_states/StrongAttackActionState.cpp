#include "mcv_platform.h"
#include "StrongAttackActionState.h"
#include "components/comp_hitbox.h"

StrongAttackActionState::StrongAttackActionState(CHandle playerModelHandle, CHandle hitbox)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
	hitboxHandle = hitbox;
}

void StrongAttackActionState::update (float delta) {
	if (isLauncher && timer.elapsed() > beginLauncherTime) {
		dbg("Cambio de strong attack a vertical launcher\n");
		GetPlayerModel()->SetAttackState(TCompPlayerModel::ActionStates::VerticalLauncher);
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
			SetPose();
			CEntity *hitboxEntity = hitboxHandle;
			TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
			hitbox->enable();
		}
	}
}

void StrongAttackActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	dbg("Strong Attack\n");
	hitboxOutTime = warmUpFrames * (1.f / 60);
	hitEndTime = hitboxOutTime + activeFrames * (1.f / 60);
	animationEndTime = hitEndTime + endingLagFrames * (1.f / 60);
	interruptibleTime = IASAFrames * (1.f / 60);
	beginLauncherTime = startLauncherFrames * (1.f / 60);
	isLauncher = true;
	velocityVector->x = 0.f;
	velocityVector->z = 0.f;
	deltaMovement.y = GetPlayerModel()->movementState->GetDeltaMovement().y;
	timer.reset();
	GetPlayerModel()->lockMovementState = true;
	GetPlayerModel()->lockWalk = true;
}

void StrongAttackActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	GetPlayerModel()->movementState->SetPose();
	dbg("Finish strong Attack\n");
}

void StrongAttackActionState::SetMovementInput(VEC2 input) {
}

void StrongAttackActionState::OnJumpHighButton() {
}

void StrongAttackActionState::OnJumpLongButton() {
}

void StrongAttackActionState::OnStrongAttackButtonReleased() {
	isLauncher = false;
}

void StrongAttackActionState::OnLeavingGround() {
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}

void StrongAttackActionState::OnHitboxEnter(CHandle entity) {
	CHandle playerEntity = playerModelHandle.getOwner();
	if (entity != playerEntity) {
		CEntity *otherEntity = entity;
		otherEntity->sendMsg(TMsgAttackHit{ playerEntity, damage });
		dbg("Strong attack hit for %i damage\n", damage);
	}
}