#include "mcv_platform.h"
#include "HorizontalLauncherActionState.h"
#include "components/comp_hitbox.h"

HorizontalLauncherActionState::HorizontalLauncherActionState(CHandle playerModelHandle, CHandle hitbox)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
	hitboxHandle = hitbox;
}

void HorizontalLauncherActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	if (phase == AttackPhases::Recovery && timer.elapsed() >= animationEndTime) {
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

void HorizontalLauncherActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	dbg("Horizontal Launcher\n");
	phase = AttackPhases::Startup;
	hitboxOutTime = warmUpFrames * (1.f / 60);
	hitEndTime = activeFrames * (1.f / 60);
	animationEndTime = endingLagFrames * (1.f / 60);
	interruptibleTime = IASAFrames * (1.f / 60);
	velocityVector->x = 0.f;
	velocityVector->z = 0.f;
	timer.reset();
	GetPlayerModel()->lockMovementState = true;
	GetPlayerModel()->lockWalk = true;
}

void HorizontalLauncherActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	GetPlayerModel()->movementState->SetPose();
	CEntity *hitboxEntity = hitboxHandle;
	TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();
	dbg("Finish Horizontal Launcher\n");
}

void HorizontalLauncherActionState::OnJumpHighButton() {
}

void HorizontalLauncherActionState::OnJumpLongButton() {
}

void HorizontalLauncherActionState::OnLeavingGround() {
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}

void HorizontalLauncherActionState::OnHitboxEnter(CHandle entity) {
	CHandle playerEntity = playerModelHandle.getOwner();
	if (entity != playerEntity) {
		CEntity *otherEntity = entity;
		otherEntity->sendMsg(TMsgAttackHit{ playerEntity, damage });
		otherEntity->sendMsg(TMsgLaunchedHorizontally{ playerEntity, damage, GetPlayerTransform()->getFront()});
		dbg("Horizontal Launcher hit for %f damage\n", damage);
	}
}