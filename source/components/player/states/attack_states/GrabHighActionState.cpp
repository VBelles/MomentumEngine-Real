#include "mcv_platform.h"
#include "GrabHighActionState.h"
#include "components/comp_hitbox.h"

GrabHighActionState::GrabHighActionState(TCompPlayerModel * player, CHandle hitbox)
	: AirborneActionState::AirborneActionState(player) {
	hitboxHandle = hitbox;
}

void GrabHighActionState::update (float delta) {
	if (timer.elapsed() > animationEndTime) {
		player->SetAttackState(TCompPlayerModel::ActionStates::Idle);
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

void GrabHighActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	dbg("ENTER grab high\n");
	hitboxOutTime = warmUpFrames * (1.f / 60);
	hitEndTime = hitboxOutTime + activeFrames * (1.f / 60);
	animationEndTime = hitEndTime + endingLagFrames * (1.f / 60);
	interruptibleTime = IASAFrames * (1.f / 60);
	timer.reset();
}

void GrabHighActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	dbg("FINISH grab high\n");
}

void GrabHighActionState::SetMovementInput(VEC2 input) {
}

void GrabHighActionState::OnJumpHighButton() {
}

void GrabHighActionState::OnJumpLongButton() {
}

void GrabHighActionState::OnLanding() {
	CEntity *hitboxEntity = hitboxHandle;
	TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();

	player->SetMovementState(TCompPlayerModel::ActionStates::Run);
	player->SetAttackState(TCompPlayerModel::ActionStates::Idle);
}

void GrabHighActionState::OnHitboxEnter(CHandle entity) {
	CHandle playerHandle = CHandle(player).getOwner();
	if (entity != playerHandle) {
		CEntity *otherEntity = entity;
		otherEntity->sendMsg(TMsgAttackHit{ playerHandle, damage });
		dbg("Grab\n", damage);
	}
}