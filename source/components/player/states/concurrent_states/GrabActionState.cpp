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
		getPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::Idle);
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

void GrabActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	phase = AttackPhases::Startup;
	setPose();
	hitboxOutTime = warmUpFrames * (1.f / 60);
	hitEndTime = activeFrames * (1.f / 60);
	animationEndTime = endingLagFrames * (1.f / 60);
	interruptibleTime = IASAFrames * (1.f / 60);
	timer.reset();
	getPlayerModel()->lockTurning = true;
}

void GrabActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
	getPlayerModel()->baseState->setPose();
	CEntity *hitboxEntity = hitboxHandle;
	TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();
	getPlayerModel()->lockTurning = false;
}

void GrabActionState::OnLanding() {
	CEntity *hitboxEntity = hitboxHandle;
	TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();

	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Landing);
	getPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::Idle);
}