#include "mcv_platform.h"
#include "HardLandingActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/comp_render.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"


HardLandingActionState::HardLandingActionState(StateManager* stateManager) :
	LandingActionState(stateManager, HardLanding) {
	landingLagTime = frames2sec(38);
}

void HardLandingActionState::update(float delta) {
	LandingActionState::update(delta);
	if (hitboxTimer.elapsed() >= impactAttackDurationTime) {
		getHitboxes()->disable(hitbox);
	}
	if (fallingAttackHitboxTimer.elapsed() >= fallingAttackTime) {
		getHitboxes()->disable(hitboxFallingAttack);
		getHitboxes()->disable(hitboxPlummet);
	}
}

void HardLandingActionState::onStateEnter(IActionState * lastState) {
	LandingActionState::onStateEnter(lastState);
	hasTriedSpringJump = false;
	movementInput = VEC2::Zero;
	getHitboxes()->enable(hitbox);
	hitboxTimer.reset();
	springJumpTimer.reset();
	fallingAttackHitboxTimer.reset();
	getSkeleton()->clear();
	getSkeleton()->executeAction(animation, 0.05f, 0.05f);
}

void HardLandingActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	getHitboxes()->disable(hitbox);
	getHitboxes()->disable(hitboxFallingAttack); //Por si las moscas
	getHitboxes()->disable(hitboxPlummet); //Por si las moscas
}

void HardLandingActionState::onJumpHighButton() {
	if (!hasTriedSpringJump && springJumpTimer.elapsed() >= springJumpWindowStartTime && springJumpTimer.elapsed() <= springJumpWindowEndTime) {
		stateManager->changeState(JumpSquatSpring);
	}
	hasTriedSpringJump = true;
}

void HardLandingActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = CHandle(stateManager->getEntity());
	CEntity *otherEntity = entity;
	TMsgAttackHit msgAtackHit = {};
	msgAtackHit.attacker = playerEntity;
	msgAtackHit.info = {};
	if (hitbox == hitboxFallingAttack) {
		otherEntity->sendMsg(TMsgGetPower{ playerEntity, fallingAttackPowerToGet });
		msgAtackHit.info.damage = fallingAttackDamage;
		msgAtackHit.info.givesPower = true;
	}
	else {
		msgAtackHit.info.stun = new AttackInfo::Stun{ stunTime };
	}

	otherEntity->sendMsg(msgAtackHit);
}

