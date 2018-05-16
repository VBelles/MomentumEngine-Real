#include "mcv_platform.h"
#include "PropelHighActionState.h"
#include "components/comp_hitbox.h"
#include "components/comp_render.h"
#include "components/player/comp_player_model.h"
#include "components/comp_camera.h"
#include "components/comp_transform.h"
#include "entity/common_msgs.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"


PropelHighActionState::PropelHighActionState(StateManager* stateManager): 
	AirborneActionState(stateManager, PropelHigh){
}


void PropelHighActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	//deltaMovement.y = velocityVector->y * delta;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	if (timer.elapsed() >= endingTime) {
		if (!isChangingBaseState) {
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
			CHandle playerEntity = CHandle(stateManager->getEntity());
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

void PropelHighActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	propelTarget = getPlayerModel()->grabTarget;
	CEntity* targetEntity = propelTarget;
	TCompTransform* targetTransform = targetEntity->get<TCompTransform>();
	movingVelocity = targetTransform->getPosition() + VEC3::Up * 2.f - getPlayerTransform()->getPosition();
	movingVelocity /= endingTime;
	timer.reset();
	getPlayerModel()->lastWallNormal = PxVec3(0, 0, 0);
	getPlayerModel()->getSkeleton()->executeAction(animation, 0.2f, 0.2f);
}

void PropelHighActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
}
