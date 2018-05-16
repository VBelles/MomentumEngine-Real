#include "mcv_platform.h"
#include "FallingAttackActionState.h"
#include "FallingAttackLandingActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_hitboxes.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"


FallingAttackActionState::FallingAttackActionState(StateManager* stateManager) :
	AirborneActionState(stateManager, FallingAttack) {
}

void FallingAttackActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;

	if (timer.elapsed() < hitboxOutTime) {
		//posicionamiento
		bool hasInput = movementInput != VEC2::Zero;
		PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();

		if (hasInput) {
			VEC3 desiredDirection = getCamera()->getCamera()->TransformToWorld(movementInput);
			VEC3 targetPos = getPlayerTransform()->getPosition() + desiredDirection;
			rotatePlayerTowards(delta, targetPos, 10.f);
		}
		velocityVector->y = 0.f;
		deltaMovement.y = upwardsVelocity * delta;
	}
	else {
		//ataque ca�da
		if (timer.elapsed() >= hitboxOutTime) {
			getHitboxes()->enable(hitbox);
		}
		getPlayerModel()->setGravityMultiplier(fallingMultiplier);
	}
}

void FallingAttackActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	getPlayerModel()->maxVerticalSpeed = maxFallingVelocity;
	getPlayerModel()->setGravity(0.f);
	*velocityVector = VEC3::Zero;
	timer.reset();
	getPlayerModel()->getSkeleton()->blendCycle(animation, 0.2f, 0.2f);
	getPlayerModel()->getSkeleton()->executeAction(animationPositioning, 0.2f, 0.2f);
}

void FallingAttackActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
	if (!dynamic_cast<FallingAttackLandingActionState*>(nextState)) {
		getHitboxes()->disable(hitbox); //que la deshabilite LandingFallingAttack si es posible
	}
	getPlayerModel()->resetGravity();
}

void FallingAttackActionState::onLanding() {
	//getHitboxes()->disable(hitbox); //que la deshabilite LandingFallingAttack
	*velocityVector = VEC3::Zero;
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::LandingFallingAttack);
	getPlayerModel()->setConcurrentState(TCompPlayerModel::ActionStates::Idle);
}


void FallingAttackActionState::onHitboxEnter(std::string hitbox, CHandle entity) {
	CHandle playerEntity = CHandle(stateManager->getEntity());
	CEntity *otherEntity = entity;

	otherEntity->sendMsg(TMsgGetPower{ playerEntity, powerToGet });
	VEC3 propelVelocity = { 0, -maxFallingVelocity, 0 };
	TMsgAttackHit msgAtackHit = {};
	msgAtackHit.attacker = playerEntity;
	msgAtackHit.info = {};
	msgAtackHit.info.givesPower = true;
	msgAtackHit.info.damage = damage;
	msgAtackHit.info.propel = new AttackInfo::Propel{ propelVelocity };
	otherEntity->sendMsg(msgAtackHit);

}