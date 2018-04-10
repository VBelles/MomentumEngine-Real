#include "mcv_platform.h"
#include "FallingAttackLandingActionState.h"
#include "components/comp_hitbox.h"
#include "components/comp_render.h"

FallingAttackLandingActionState::FallingAttackLandingActionState(CHandle playerModelHandle, CHandle hitbox)
	: LandingActionState::LandingActionState(playerModelHandle) {
	hitboxHandle = hitbox;
}

void FallingAttackLandingActionState::update(float delta) {
	LandingActionState::update(delta);
	if (hitboxTimer.elapsed() >= impactAttackDurationTime) {
		CEntity *hitboxEntity = hitboxHandle;
		TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
		hitbox->disable();
	}
}

void FallingAttackLandingActionState::onStateEnter(IActionState * lastState) {
	LandingActionState::onStateEnter(lastState);
	landingLagTime = newLandingLagFrames * (1.f / 60);
	impactAttackDurationTime = impactAttackDurationFrames * (1.f / 60);
	springJumpWindowStartTime = springJumpWindowStartFrame * (1.f / 60);
	springJumpWindowEndTime = springJumpWindowEndFrame * (1.f / 60);
	hasTriedSpringJump = false;
	movementInput = VEC2::Zero;
	CEntity *hitboxEntity = hitboxHandle;
	TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->enable();
	hitboxTimer.reset();
	springJumpTimer.reset();
}

void FallingAttackLandingActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	//dbg("Saliendo de falling attack landing\n");
	CEntity *hitboxEntity = hitboxHandle;
	TCompHitbox *hitbox = hitboxEntity->get<TCompHitbox>();
	hitbox->disable();
}

void FallingAttackLandingActionState::onJumpHighButton() {
	if (!hasTriedSpringJump && springJumpTimer.elapsed() >= springJumpWindowStartTime && springJumpTimer.elapsed() <= springJumpWindowEndTime) {
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::JumpSquatSpring);
	}
	hasTriedSpringJump = true;
}

void FallingAttackLandingActionState::onHitboxEnter(CHandle entity) {
	CHandle playerEntity = playerModelHandle.getOwner();
	if (entity != playerEntity) {
		CEntity *otherEntity = entity;
		TMsgAttackHit msgAtackHit = {};
		msgAtackHit.attacker = playerEntity;
		msgAtackHit.info = {};
		msgAtackHit.info.stun = new AttackInfo::Stun{ stunTime };
		otherEntity->sendMsg(msgAtackHit);
	}
}

void FallingAttackLandingActionState::setPose() {
	getRender()->setMesh("data/meshes/pose_landing.mesh");
}
