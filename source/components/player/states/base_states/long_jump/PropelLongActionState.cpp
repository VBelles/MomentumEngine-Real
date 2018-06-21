#include "mcv_platform.h"
#include "PropelLongActionState.h"
#include "components/comp_hitbox.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "components/player/comp_player_model.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"


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
			CHandle playerEntity = CHandle(stateManager->getEntity());
			CEntity* targetEntity = propelTarget;
			VEC3 propelVelocity = -getPlayerTransform()->getFront() * currentPowerStats->longJumpVelocityVector.z;
			TMsgAttackHit msgAtackHit = {};
			msgAtackHit.attacker = playerEntity;
			msgAtackHit.info = {};
			msgAtackHit.info.propel = new AttackInfo::Propel{
				propelVelocity
			};
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
		getSkeleton()->executeAction(animation, 0.2f, 0.2f);
		getPlayerModel()->lastWallNormal = PxVec3(0, 0, 0);
	}
	else {
		stateManager->changeState(AirborneNormal);
	}
}

void PropelLongActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
}

