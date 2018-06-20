#include "mcv_platform.h"
#include "StrongAttack3ActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"


StrongAttack3ActionState::StrongAttack3ActionState(StateManager * stateManager) :
	GroundedActionState(stateManager, StrongAttack3),
	AttackState(stateManager) {
	hitboxOutTime = frames2sec(25);
	hitEndTime = frames2sec(10);
	animationEndTime = frames2sec(50);
	cancelableTime = frames2sec(20);
	interruptibleTime = frames2sec(85);
	hitbox = "strong_attack";
}

void StrongAttack3ActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;

	AttackState::update(delta);

	if (phase == AttackPhases::Startup) {
		//posicionamiento
		bool hasInput = movementInput.Length() > PAD_DEAD_ZONE;

		if (hasInput) {
			VEC3 desiredDirection = getCamera()->getCamera()->TransformToWorld(movementInput);
			VEC3 targetPos = getPlayerTransform()->getPosition() + desiredDirection;
			rotatePlayerTowards(delta, targetPos, 3.f);
		}
	}
}

void StrongAttack3ActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	AttackState::onStateEnter(lastState);
	dbg("Strong 3\n");
	getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	*velocityVector = VEC3::Zero;
	stateManager->changeConcurrentState(Free);
}

void StrongAttack3ActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	AttackState::onStateExit(nextState); 
	getSkeleton()->removeAction(animation, 0.2f);
}

void StrongAttack3ActionState::onDodgeButton() {
	if (isCancelable() || isInterruptible()) GroundedActionState::onDodgeButton();
}

void StrongAttack3ActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
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