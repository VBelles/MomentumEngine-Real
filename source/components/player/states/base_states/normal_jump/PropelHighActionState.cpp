#include "mcv_platform.h"
#include "PropelHighActionState.h"
#include "components/comp_hitbox.h"
#include "components/comp_render.h"
#include "components/player/comp_player_model.h"
#include "components/comp_camera.h"
#include "components/comp_transform.h"
#include "entity/common_msgs.h"

PropelHighActionState::PropelHighActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
}

void PropelHighActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	//deltaMovement.y = velocityVector->y * delta;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	if (timer.elapsed() >= endingTime) {
		if (movementInput != VEC2::Zero) {
			VEC3 inputDirection = getCamera()->TransformToWorld(movementInput);
			float newYaw = atan2(inputDirection.x, inputDirection.z);
			float y, p, r;
			getPlayerTransform()->getYawPitchRoll(&y, &p, &r);
			getPlayerTransform()->setYawPitchRoll(newYaw, p, r);
		}
		velocityVector->y = currentPowerStats->jumpVelocityVector.y;
		deltaMovement = *velocityVector * delta;
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
		//pasar mensaje a la otra entidad
		CHandle playerEntity = CHandle(playerModelHandle).getOwner();
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
	else {
		*velocityVector = VEC3::Zero;
		deltaMovement = movingVelocity * delta;
	}
}

void PropelHighActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	setPose();
	endingTime = endingFrames * (1.f / 60);
	propelTarget = getPlayerModel()->grabTarget;
	CEntity* targetEntity = propelTarget;
	TCompTransform* targetTransform = targetEntity->get<TCompTransform>();
	movingVelocity = targetTransform->getPosition() + VEC3::Up * 2.f - getPlayerTransform()->getPosition();
	movingVelocity /= endingTime;
	timer.reset();
	getPlayerModel()->lastWallEntered = nullptr;
	getPlayerModel()->lastWallNormal = PxVec3(0, 0, 0);
}

void PropelHighActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
	getPlayerModel()->baseState->setPose();
}

void PropelHighActionState::setPose() {
	getRender()->setMesh("data/meshes/pose_propel.mesh");
}
