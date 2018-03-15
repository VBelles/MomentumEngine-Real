#include "mcv_platform.h"
#include "GrabHighActionState.h"
#include "components/comp_hitbox.h"

GrabActionState::GrabActionState(CHandle playerModelHandle, CHandle hitbox)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
	hitboxHandle = hitbox;
}

void GrabActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	if (phase == AttackPhases::Recovery && timer.elapsed() >= animationEndTime) {
		GetPlayerModel()->SetAttackState(TCompPlayerModel::ActionStates::Idle);
	}
	else if (phase == AttackPhases::Active && timer.elapsed() >= hitEndTime) {
		timer.reset();
		CEntity *hitboxEntity = hitboxHandle;
		TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
		hitbox->disable();
		phase = AttackPhases::Recovery;
	}
	else if (phase == AttackPhases::Startup && timer.elapsed() >= hitboxOutTime) {
		timer.reset();
		CEntity *hitboxEntity = hitboxHandle;
		TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
		hitbox->enable();
		phase = AttackPhases::Active;
	}
}

void GrabActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	phase = AttackPhases::Startup;
	SetPose();
	hitboxOutTime = warmUpFrames * (1.f / 60);
	hitEndTime = activeFrames * (1.f / 60);
	animationEndTime = endingLagFrames * (1.f / 60);
	interruptibleTime = IASAFrames * (1.f / 60);
	timer.reset();
}

void GrabActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	GetPlayerModel()->movementState->SetPose();
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

	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::Landing);
	GetPlayerModel()->SetAttackState(TCompPlayerModel::ActionStates::Idle);
}

void GrabActionState::OnHitboxEnter(CHandle entity) {

}