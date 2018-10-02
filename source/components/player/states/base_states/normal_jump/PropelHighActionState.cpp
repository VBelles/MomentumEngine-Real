#include "mcv_platform.h"
#include "PropelHighActionState.h"


PropelHighActionState::PropelHighActionState(StateManager* stateManager) :
	AirborneActionState(stateManager, PropelHigh) {
}


void PropelHighActionState::update(float delta) {
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
			velocityVector->y = currentPowerStats->jumpVelocityVector.y;
			deltaMovement = *velocityVector * delta;
			stateManager->changeState(AirborneNormal);
			//pasar mensaje a la otra entidad
			CHandle playerEntity = getPlayerEntity();
			CEntity* targetEntity = propelTarget;
			VEC3 propelVelocity = { 0, -currentPowerStats->jumpVelocityVector.y, 0 };
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

void PropelHighActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	propelTarget = getPlayerModel()->grabTarget;
	if (propelTarget.isValid()) {
		CEntity* targetEntity = propelTarget;
		TCompTransform* targetTransform = targetEntity->get<TCompTransform>();
		movingVelocity = targetTransform->getPosition() + VEC3::Up * 2.f - getPlayerTransform()->getPosition();
		movingVelocity /= endingTime;
		timer.reset();
		getPlayerModel()->lastWallNormal = PxVec3(0, 0, 0);
		getSkeleton()->executeAction(animation, 0.03f, 0.2f);
		getPlayerModel()->makeInvulnerable(invulnerableTime);
	}
	else {
		stateManager->changeState(AirborneNormal);
	}
}

void PropelHighActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
}
