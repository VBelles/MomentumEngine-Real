#include "mcv_platform.h"
#include "ReleasePowerAirActionState.h"
#include "components/comp_hitbox.h"

ReleasePowerAirActionState::ReleasePowerAirActionState(CHandle playerModelHandle, CHandle hitboxSmall, CHandle hitboxBig)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
	hitboxSmallHandle = hitboxSmall;
	hitboxBigHandle = hitboxBig;
}

void ReleasePowerAirActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
}

void ReleasePowerAirActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	phase = AttackPhases::Startup;
	GetPlayerModel()->GetPowerGauge()->ReleasePower();
	SetPose();
	hitboxOutTime = warmUpFrames * (1.f / 60);
	hitEndTime = activeFrames * (1.f / 60);
	animationEndTime = endingLagFrames * (1.f / 60);
	interruptibleTime = IASAFrames * (1.f / 60);
	timer.reset();
}

void ReleasePowerAirActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	GetPlayerModel()->baseState->SetPose();
	CEntity *hitboxEntity = hitboxSmallHandle;
	TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();
	hitboxEntity = hitboxBigHandle;
	hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();
}

void ReleasePowerAirActionState::OnReleasePowerButton() {
}

void ReleasePowerAirActionState::OnLanding() {
	GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::Landing);
	GetPlayerModel()->SetConcurrentState(TCompPlayerModel::ActionStates::Idle);
}

void ReleasePowerAirActionState::OnHitboxEnter(CHandle entity) {

}