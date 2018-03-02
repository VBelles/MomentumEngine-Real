#include "mcv_platform.h"
#include "HorizontalLauncherActionState.h"
#include "components/comp_hitbox.h"

HorizontalLauncherActionState::HorizontalLauncherActionState(CHandle playerModelHandle, CHandle hitbox)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
	hitboxHandle = hitbox;
}

void HorizontalLauncherActionState::update (float delta) {
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
		GetRender()->setMesh("data/meshes/pose_punch.mesh");
		CEntity *hitboxEntity = hitboxHandle;
		TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
		hitbox->enable();
	}
}

void HorizontalLauncherActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	dbg("Horizontal Launcher\n");
	hitboxOutTime = warmUpFrames * (1.f / 60);
	hitEndTime = hitboxOutTime + activeFrames * (1.f / 60);
	animationEndTime = hitEndTime + endingLagFrames * (1.f / 60);
	interruptibleTime = IASAFrames * (1.f / 60);
	velocityVector->x = 0.f;
	velocityVector->z = 0.f;
	timer.reset();
	GetPlayerModel()->lockMovementState = true;
	GetPlayerModel()->lockWalk = true;
}

void HorizontalLauncherActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	dbg("Finish Horizontal Launcher\n");
}

void HorizontalLauncherActionState::SetMovementInput(VEC2 input) {
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