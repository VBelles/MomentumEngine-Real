#include "mcv_platform.h"
#include "FallingAttackLandingActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/comp_render.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"


FallingAttackLandingActionState::FallingAttackLandingActionState(StateManager* stateManager) :
	LandingActionState(stateManager, LandingFallingAttack) {
}

void FallingAttackLandingActionState::update(float delta) {
	LandingActionState::update(delta);
	if (hitboxTimer.elapsed() >= impactAttackDurationTime) {
		getHitboxes()->disable(hitbox);
	}
	if (fallingAttackHitboxTimer.elapsed() >= fallingAttackTime) {
		getHitboxes()->disable(hitboxFallingAttack);
		getHitboxes()->disable(hitboxPlummet);
	}
}

void FallingAttackLandingActionState::onStateEnter(IActionState * lastState) {
	LandingActionState::onStateEnter(lastState);
	landingLagTime = newLandingLagTime;
	hasTriedSpringJump = false;
	movementInput = VEC2::Zero;
	getHitboxes()->enable(hitbox);
	hitboxTimer.reset();
	springJumpTimer.reset();
	fallingAttackHitboxTimer.reset();
	getSkeleton()->clearActions();
	getSkeleton()->executeAction(animation, 0.2f, 0.2f);
}

void FallingAttackLandingActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	getHitboxes()->disable(hitbox);
	getHitboxes()->disable(hitboxFallingAttack); //Por si las moscas
	getHitboxes()->disable(hitboxPlummet); //Por si las moscas
}

void FallingAttackLandingActionState::onJumpHighButton() {
	if (!hasTriedSpringJump && springJumpTimer.elapsed() >= springJumpWindowStartTime && springJumpTimer.elapsed() <= springJumpWindowEndTime) {
		stateManager->changeState(JumpSquatSpring);
	}
	hasTriedSpringJump = true;
}

void FallingAttackLandingActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = CHandle(stateManager->getEntity());
	CEntity *otherEntity = entity;
	TMsgAttackHit msgAtackHit = {};
	msgAtackHit.attacker = playerEntity;
	msgAtackHit.info = {};
	if (hitbox == hitboxFallingAttack) {
		//dbg("Those nice frames\n");
		otherEntity->sendMsg(TMsgGetPower{ playerEntity, fallingAttackPowerToGet });
		msgAtackHit.info.damage = fallingAttackDamage;
		msgAtackHit.info.givesPower = true;
	}
	else {
		//dbg("Landing\n");
		msgAtackHit.info.stun = new AttackInfo::Stun{ stunTime };
	}

	otherEntity->sendMsg(msgAtackHit);
}

