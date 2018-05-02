#include "mcv_platform.h"
#include "PropelLongActionState.h"
#include "components/comp_hitbox.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "components/player/comp_player_model.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"


void PropelLongActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	//deltaMovement.y = velocityVector->y * delta;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	if (timer.elapsed() >= endingTime) {
		if (!isChangingBaseState) {
			if (movementInput != VEC2::Zero) {
				VEC3 inputDirection = getCamera()->TransformToWorld(movementInput);
				float newYaw = atan2(inputDirection.x, inputDirection.z);
				float y, p, r;
				getPlayerTransform()->getYawPitchRoll(&y, &p, &r);
				getPlayerTransform()->setYawPitchRoll(newYaw, p, r);
			}
			*velocityVector = getPlayerTransform()->getFront() * currentPowerStats->longJumpVelocityVector.z;
			velocityVector->y = currentPowerStats->longJumpVelocityVector.y;
			deltaMovement = *velocityVector * delta;

			getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneLong);
			//pasar mensaje a la otra entidad
			CHandle playerEntity = playerModelHandle.getOwner();
			CEntity* targetEntity = propelTarget;
			VEC3 propelVelocity = { 0, -currentPowerStats->jumpVelocityVector.y, 0 };
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
	endingTime = endingFrames * (1.f / 60);
	propelTarget = getPlayerModel()->grabTarget;
	CEntity* targetEntity = propelTarget;
	TCompTransform* targetTransform = targetEntity->get<TCompTransform>();
	movingVelocity = targetTransform->getPosition() + VEC3::Up * 2.f - getPlayerTransform()->getPosition();
	movingVelocity /= endingTime;
	timer.reset();
	getPlayerModel()->getSkeleton()->executeAction(animation, 0.2f, 0.2f);
	getPlayerModel()->lastWallEntered = nullptr;
	getPlayerModel()->lastWallNormal = PxVec3(0, 0, 0);
}

void PropelLongActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
	//getPlayerModel()->baseState->setPose();
}

