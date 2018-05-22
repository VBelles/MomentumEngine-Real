#include "mcv_platform.h"
#include "FastAttack2ActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_render.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_hitboxes.h"
#include "components/player/states/StateManager.h"


FastAttack2ActionState::FastAttack2ActionState(StateManager* stateManager)
	: GroundedActionState(stateManager, FastAttack2),
	AttackState(stateManager) {
	cancelableTime = frames2sec(9);;
	interruptibleTime = frames2sec(30);
}

void FastAttack2ActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	if (phase == AttackPhases::Launch && timer.elapsed() >= beginLauncherTime) {
		stateManager->changeConcurrentState(Free);
		stateManager->changeState(HorizontalLauncher);
	}
	else if (phase == AttackPhases::Recovery && timer.elapsed() >= animationEndTime) {
		stateManager->changeConcurrentState(Free);
	}
	else if (phase == AttackPhases::Active && timer.elapsed() >= hitEndTime) {
		timer.reset();
		getHitboxes()->disable(hitbox);
		phase = AttackPhases::Recovery;
	}
	else if (phase == AttackPhases::Startup && timer.elapsed() >= hitboxOutTime) {
		timer.reset();
		getHitboxes()->enable(hitbox);
		phase = AttackPhases::Active;
	}
}

void FastAttack2ActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	AttackState::onStateEnter(lastState);
	dbg("Fast attack 2\n");
	phase = AttackPhases::Launch;
	timer.reset();
	getPlayerModel()->lockWalk = false;
	getPlayerModel()->lockAttack = true;
}

void FastAttack2ActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	AttackState::onStateExit(nextState);
	getHitboxes()->disable(hitbox);
	getPlayerModel()->lockAttack = false;
	getPlayerModel()->lockWalk = false;
	getSkeleton()->removeAction(animation, 0.2f);
}

void FastAttack2ActionState::onFastAttackButton() {
	if (isInterruptible()) stateManager->changeConcurrentState(FastAttack);
}

void FastAttack2ActionState::onStrongAttackButton() {
	if (isInterruptible()) stateManager->changeState(StrongFinisher2);
}

void FastAttack2ActionState::onDodgeButton() {
	if (isCancelable() || isInterruptible()) stateManager->changeState(Dodge);
}

void FastAttack2ActionState::onReleasePowerButton() {
	if (isInterruptible()) GroundedActionState::onReleasePowerButton();
}

void FastAttack2ActionState::onFastAttackButtonReleased() {
	if (phase == AttackPhases::Launch) {
		phase = AttackPhases::Startup;
		getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	}
}

void FastAttack2ActionState::onLeavingGround() {
	stateManager->changeState(GhostJumpWindow);
}


void FastAttack2ActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = CHandle(stateManager->getEntity());
	CEntity* otherEntity = entity;

	otherEntity->sendMsg(TMsgGetPower{ playerEntity, powerToGet });

	TMsgAttackHit msgAtackHit = {};
	msgAtackHit.attacker = playerEntity;
	msgAtackHit.info = {};
	msgAtackHit.info.givesPower = true;
	msgAtackHit.info.damage = damage;
	otherEntity->sendMsg(msgAtackHit);

}