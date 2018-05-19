#include "mcv_platform.h"
#include "StrongAttackActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"


StrongAttackActionState::StrongAttackActionState(StateManager * stateManager) :
	GroundedActionState(stateManager, StrongAttack),
	AttackState(stateManager) {
	hitboxOutTime = frames2sec(60);
	hitEndTime = frames2sec(10);
	animationEndTime = frames2sec(60);
	cancelableTime = frames2sec(50);
	interruptibleTime = frames2sec(110);
	hitbox = "strong_attack";
}

void StrongAttackActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;

	if (phase == AttackPhases::Launch && timer.elapsed() >= beginLauncherTime) {
		if (!stateManager->isChangingBaseState) {
			stateManager->changeState(VerticalLauncher);
		}
	}
	else {
		AttackState::update(delta);
	}

	if (phase == AttackPhases::Startup || phase == AttackPhases::Launch) {
		//posicionamiento
		bool hasInput = movementInput != VEC2::Zero;

		if (hasInput) {
			VEC3 desiredDirection = getCamera()->getCamera()->TransformToWorld(movementInput);
			VEC3 targetPos = getPlayerTransform()->getPosition() + desiredDirection;
			rotatePlayerTowards(delta, targetPos, 3.f);
		}
	}
}

void StrongAttackActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	AttackState::onStateEnter(lastState);
	phase = AttackPhases::Launch;
	*velocityVector = VEC3::Zero;
}

void StrongAttackActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	AttackState::onStateExit(nextState);
}

void StrongAttackActionState::onStrongAttackButtonReleased() {
	if (phase == AttackPhases::Launch) {
		phase = AttackPhases::Startup;
		getPlayerModel()->getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	}
}

void StrongAttackActionState::onLeavingGround() {
	stateManager->changeState(GhostJumpWindow);
}

void StrongAttackActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
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