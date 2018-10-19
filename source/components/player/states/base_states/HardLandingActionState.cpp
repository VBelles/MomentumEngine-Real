#include "mcv_platform.h"
#include "HardLandingActionState.h"


HardLandingActionState::HardLandingActionState(StateManager* stateManager) :
	LandingActionState(stateManager, HardLanding) {
	landingLagTime = frames2sec(50);
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
	getSkeleton()->executeAction(animation, 0.05f, 0.05f);
	getParticles()->launch("hard_landing");
	getParticles()->launch("hard_landing_fire");
}

void HardLandingActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	getSkeleton()->removeAction(animation, 0.4f);
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
	CHandle playerEntity = getPlayerEntity();
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

