#include "mcv_platform.h"
#include "FallingAttackLandingActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/comp_render.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"



void FallingAttackLandingActionState::update(float delta) {
	LandingActionState::update(delta);
	if (hitboxTimer.elapsed() >= impactAttackDurationTime) {
		getHitboxes()->disable(hitbox);
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
	getHitboxes()->enable(hitbox);
	hitboxTimer.reset();
	springJumpTimer.reset();
	getPlayerModel()->getSkeleton()->executeAction(animation, 0.2f, 0.2f);
}

void FallingAttackLandingActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	getHitboxes()->disable(hitbox);
}

void FallingAttackLandingActionState::onJumpHighButton() {
	if (!hasTriedSpringJump && springJumpTimer.elapsed() >= springJumpWindowStartTime && springJumpTimer.elapsed() <= springJumpWindowEndTime) {
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::JumpSquatSpring);
	}
	hasTriedSpringJump = true;
}

void FallingAttackLandingActionState::onHitboxEnter(CHandle entity) {
	CHandle playerEntity = playerModelHandle.getOwner();
	CEntity *otherEntity = entity;
	TMsgAttackHit msgAtackHit = {};
	msgAtackHit.attacker = playerEntity;
	msgAtackHit.info = {};
	msgAtackHit.info.stun = new AttackInfo::Stun{ stunTime };
	otherEntity->sendMsg(msgAtackHit);
}

void FallingAttackLandingActionState::setPose() {
	getRender()->setMesh("data/meshes/pose_landing.mesh");
}
