#include "mcv_platform.h"
#include "StrongAttack2ActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"


StrongAttack2ActionState::StrongAttack2ActionState(StateManager * stateManager) :
	GroundedActionState(stateManager, StrongAttack2),
	AttackState(stateManager) {
	hitboxOutTime = frames2sec(10);
	hitEndTime = frames2sec(7);
	animationEndTime = frames2sec(78);
	cancelableTime = frames2sec(2);
	interruptibleTime = frames2sec(34);
	walkableTime = frames2sec(65);
	hitbox = "strong_attack2";
}

void StrongAttack2ActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;

	AttackState::update(delta);

	if (phase == AttackPhases::Startup){
		//posicionamiento
		bool hasInput = movementInput.Length() > PAD_DEAD_ZONE;

		if (hasInput) {
			VEC3 desiredDirection = getCamera()->getCamera()->TransformToWorld(movementInput);
			VEC3 targetPos = getPlayerTransform()->getPosition() + desiredDirection;
			rotatePlayerTowards(delta, targetPos, 3.f);
		}
	}
}

void StrongAttack2ActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	AttackState::onStateEnter(lastState);
	dbg("Strong 2\n");
	getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	*velocityVector = VEC3::Zero;
	stateManager->changeConcurrentState(Free);
}

void StrongAttack2ActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	AttackState::onStateExit(nextState); 
	getSkeleton()->removeAction(animation, 0.2f);
}

void StrongAttack2ActionState::onStrongAttackButton() {
	if (isInterruptible()) stateManager->changeState(StrongAttack3);
}

void StrongAttack2ActionState::onFastAttackButton() {
	if (isInterruptible()) stateManager->changeState(FastFinisher2);
}

void StrongAttack2ActionState::setMovementInput(VEC2 input) {
	IActionState::setMovementInput(input);
	if (input.Length() > PAD_DEAD_ZONE) {
		if (canWalk()) stateManager->changeState(Walk);
	}
}

void StrongAttack2ActionState::onDodgeButton() {
	if(isCancelable() || isInterruptible()) GroundedActionState::onDodgeButton();
}

void StrongAttack2ActionState::onReleasePowerButton() {
	if (isInterruptible()) GroundedActionState::onReleasePowerButton();

}

void StrongAttack2ActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = CHandle(stateManager->getEntity());
	CEntity *otherEntity = entity;
	otherEntity->sendMsg(TMsgGetPower{ playerEntity, powerToGet });
	TMsgAttackHit msgAtackHit = {};
	msgAtackHit.attacker = playerEntity;
	msgAtackHit.info = {};
	msgAtackHit.info.givesPower = true;
	msgAtackHit.info.damage = damage;
	otherEntity->sendMsg(msgAtackHit);

}