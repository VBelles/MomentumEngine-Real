#include "mcv_platform.h"
#include "FallingAttackLandingActionState.h"
#include "components/comp_hitbox.h"

FallingAttackLandingActionState::FallingAttackLandingActionState(CHandle playerModelHandle, CHandle hitbox)
	: LandingActionState::LandingActionState(playerModelHandle) {
	hitboxHandle = hitbox;
}

void FallingAttackLandingActionState::update (float delta) {
	LandingActionState::update(delta);
	if (hitboxTimer.elapsed() >= impactAttackDurationTime) {
		CEntity *hitboxEntity = hitboxHandle;
		TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
		hitbox->disable();
	}
	/*else {
		CEntity *hitboxEntity = hitboxHandle;
		TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
		hitbox->enable();
	}*/
}

void FallingAttackLandingActionState::OnStateEnter(IActionState * lastState) {
	LandingActionState::OnStateEnter(lastState);
	landingLagTime = newLandingLagFrames * (1.f / 60);
	impactAttackDurationTime = impactAttackDurationFrames * (1.f / 60);
	movementInput = VEC2::Zero;
	CEntity *hitboxEntity = hitboxHandle;
	TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->enable();
	hitboxTimer.reset();
	dbg("Entrando en falling attack landing\n");
}

void FallingAttackLandingActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	dbg("Saliendo de falling attack landing\n");
}

void FallingAttackLandingActionState::SetMovementInput(VEC2 input) {
}

void FallingAttackLandingActionState::OnJumpHighButton() {
}

void FallingAttackLandingActionState::OnJumpLongButton() {
}

void FallingAttackLandingActionState::OnHitboxEnter(CHandle entity) {
	CHandle playerEntity = playerModelHandle.getOwner();
	if (entity != playerEntity) {
		CEntity *otherEntity = entity;
		otherEntity->sendMsg(TMsgAttackHit{ playerEntity, damage });
	}
}
