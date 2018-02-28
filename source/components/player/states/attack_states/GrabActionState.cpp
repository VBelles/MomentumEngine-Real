#include "mcv_platform.h"
#include "GrabHighActionState.h"
#include "components/comp_hitbox.h"

GrabActionState::GrabActionState(CHandle playerHandle, CHandle hitbox)
	: AirborneActionState::AirborneActionState(playerHandle) {
	hitboxHandle = hitbox;
}

void GrabActionState::update (float delta) {
	if (timer.elapsed() >= animationEndTime) {
		GetPlayer()->SetAttackState(TCompPlayerModel::ActionStates::Idle);
	}
	else if (timer.elapsed() >= hitEndTime) {
		CEntity *hitboxEntity = hitboxHandle;
		TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
		hitbox->disable();
	}
	else if (timer.elapsed() >= hitboxOutTime) {
		CEntity *hitboxEntity = hitboxHandle;
		TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
		hitbox->enable();
	}
}

void GrabActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	hitboxOutTime = warmUpFrames * (1.f / 60);
	hitEndTime = hitboxOutTime + activeFrames * (1.f / 60);
	animationEndTime = hitEndTime + endingLagFrames * (1.f / 60);
	interruptibleTime = IASAFrames * (1.f / 60);
	timer.reset();
}

void GrabActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	CEntity *hitboxEntity = hitboxHandle;
	TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();
}

void GrabActionState::SetMovementInput(VEC2 input) {
}

void GrabActionState::OnJumpHighButton() {
}

void GrabActionState::OnJumpLongButton() {
}

void GrabActionState::OnLanding() {
	CEntity *hitboxEntity = hitboxHandle;
	TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();

	GetPlayer()->SetMovementState(TCompPlayerModel::ActionStates::Run);
	GetPlayer()->SetAttackState(TCompPlayerModel::ActionStates::Idle);
}

void GrabActionState::OnHitboxEnter(CHandle entity) {

}