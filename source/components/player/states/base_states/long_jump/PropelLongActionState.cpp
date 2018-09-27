#include "mcv_platform.h"
#include "PropelLongActionState.h"


PropelLongActionState::PropelLongActionState(StateManager* stateManager) :
	AirborneActionState(stateManager, PropelLong) {
}

void PropelLongActionState::update(float delta) {
	if (!propelTarget.isValid()) {
		stateManager->changeState(AirborneNormal);
		return;
	}
	deltaMovement = VEC3::Zero;
	//deltaMovement.y = velocityVector->y * delta;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	if (timer.elapsed() >= endingTime) {
		if (!stateManager->isChangingBaseState) {
			if (movementInput != VEC2::Zero) {
				VEC3 inputDirection = getCamera()->getCamera()->TransformToWorld(movementInput);
				float newYaw = atan2(inputDirection.x, inputDirection.z);
				float y, p, r;
				getPlayerTransform()->getYawPitchRoll(&y, &p, &r);
				getPlayerTransform()->setYawPitchRoll(newYaw, p, r);
			}
			*velocityVector = getPlayerTransform()->getFront() * currentPowerStats->longJumpVelocityVector.z;
			velocityVector->y = currentPowerStats->longJumpVelocityVector.y;
			deltaMovement = *velocityVector * delta;

			stateManager->changeState(AirborneLong);
			//pasar mensaje a la otra entidad
			CHandle playerEntity = getPlayerEntity();
			CEntity* targetEntity = propelTarget;
			VEC3 propelVelocity = -getPlayerTransform()->getFront() * currentPowerStats->longJumpVelocityVector.z;
			TMsgAttackHit msgAtackHit = {};
			msgAtackHit.attacker = playerEntity;
			msgAtackHit.info = {};
			msgAtackHit.info.propel = new AttackInfo::Propel{ propelDuration, propelVelocity };
			targetEntity->sendMsg(msgAtackHit);
		}
	}
	else {
		*velocityVector = VEC3::Zero;
		deltaMovement = movingVelocity * delta;
	}
}

void PropelLongActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	propelTarget = getPlayerModel()->grabTarget;
	if (propelTarget.isValid()) {
		CEntity* targetEntity = propelTarget;
		TCompTransform* targetTransform = targetEntity->get<TCompTransform>();
		movingVelocity = targetTransform->getPosition() + VEC3::Up * 2.f - getPlayerTransform()->getPosition();
		movingVelocity /= endingTime;
		timer.reset();
		getSkeleton()->executeAction(animation, 0.03f, 0.2f);
		getPlayerModel()->lastWallNormal = PxVec3(0, 0, 0);
		getPlayerModel()->makeInvulnerable(invulnerableTime);
	}
	else {
		stateManager->changeState(AirborneNormal);
	}
}

void PropelLongActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
}

