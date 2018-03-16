#include "mcv_platform.h"
#include "FastAttackActionState.h"
#include "components/comp_hitbox.h"

FastAttackActionState::FastAttackActionState(CHandle playerModelHandle, CHandle hitbox)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
	hitboxHandle = hitbox;
}

void FastAttackActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	if (phase == AttackPhases::Launch && timer.elapsed() >= beginLauncherTime) {
		GetPlayerModel()->SetAttackState(TCompPlayerModel::ActionStates::HorizontalLauncher);
	}
	else if (phase == AttackPhases::Recovery && timer.elapsed() >= animationEndTime) {
		GetPlayerModel()->SetAttackState(TCompPlayerModel::ActionStates::Idle);
		GetPlayerModel()->lockMovementState = false;
		GetPlayerModel()->lockWalk = false;
	}
	else if (phase == AttackPhases::Active && timer.elapsed() >= hitEndTime) {
		timer.reset();
		CEntity *hitboxEntity = hitboxHandle;
		TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
		hitbox->disable();
		phase = AttackPhases::Recovery;
	}
	else if (phase == AttackPhases::Startup && timer.elapsed() >= hitboxOutTime) {
		SetPose();
		timer.reset();
		CEntity *hitboxEntity = hitboxHandle;
		TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
		hitbox->enable();
		phase = AttackPhases::Active;
	}
}

void FastAttackActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	dbg("Fast Attack\n");
	phase = AttackPhases::Launch;
	hitboxOutTime = warmUpFrames * (1.f / 60);
	hitEndTime = activeFrames * (1.f / 60);
	animationEndTime = endingLagFrames * (1.f / 60);
	interruptibleTime = IASAFrames * (1.f / 60);
	beginLauncherTime = startLauncherFrames * (1.f / 60);
	timer.reset();
	GetPlayerModel()->lockMovementState = true;
	GetPlayerModel()->lockWalk = false;
}

void FastAttackActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	GetPlayerModel()->movementState->SetPose();
	CEntity *hitboxEntity = hitboxHandle;
	TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();
	dbg("Finish fast Attack\n");
}

void FastAttackActionState::SetMovementInput(VEC2 input) {
}

void FastAttackActionState::OnJumpHighButton() {
}

void FastAttackActionState::OnJumpLongButton() {
}

void FastAttackActionState::OnFastAttackButtonReleased() {
	phase = AttackPhases::Startup;

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