#include "mcv_platform.h"
#include "FastAttackActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_render.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_hitboxes.h"
#include "components/player/states/StateManager.h"
#include "components/comp_transform.h"


FastAttackActionState::FastAttackActionState(StateManager* stateManager)
	: GroundedActionState(stateManager, FastAttack),
	AttackState(stateManager) {
	cancelableTime = frames2sec(9);;
	interruptibleTime = frames2sec(26);
}

void FastAttackActionState::update(float delta) {
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

void FastAttackActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	AttackState::onStateEnter(lastState);
	dbg("Fast attack 1\n");
	phase = AttackPhases::Launch;
	timer.reset();
	getPlayerModel()->lockWalk = false;
	getPlayerModel()->lockAttack = true;
}

void FastAttackActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	AttackState::onStateExit(nextState);
	getHitboxes()->disable(hitbox);
	getPlayerModel()->lockAttack = false;
	getPlayerModel()->lockWalk = false;
	getSkeleton()->removeAction(animation, 0.2f);
}

void FastAttackActionState::onFastAttackButton() {
	if (isInterruptible()) stateManager->changeConcurrentState(FastAttack2);
}

void FastAttackActionState::onStrongAttackButton() {
	if (isInterruptible()) stateManager->changeState(StrongFinisher1);
}

void FastAttackActionState::onDodgeButton() {
	if (isCancelable() || isInterruptible()) stateManager->changeState(Dodge);
}

void FastAttackActionState::onReleasePowerButton() {
	if (isInterruptible()) GroundedActionState::onReleasePowerButton();
}

void FastAttackActionState::onFastAttackButtonReleased() {
	if (phase == AttackPhases::Launch) {
		phase = AttackPhases::Startup;
		getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	}
}

void FastAttackActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = CHandle(stateManager->getEntity());
	CEntity* otherEntity = entity;

	otherEntity->sendMsg(TMsgGetPower{ playerEntity, powerToGet });

	TMsgAttackHit msgAtackHit = {};
	msgAtackHit.attacker = playerEntity;
	msgAtackHit.info = {};
	VEC3 launchVelocity = getPlayerTransform()->getFront() * launchSpeed.x;
	launchVelocity.y = launchSpeed.y;
	msgAtackHit.info.horizontalLauncher = new AttackInfo::HorizontalLauncher{
		suspensionTime,
		launchVelocity
	};
	msgAtackHit.info.givesPower = true;
	msgAtackHit.info.damage = damage;
	otherEntity->sendMsg(msgAtackHit);

}