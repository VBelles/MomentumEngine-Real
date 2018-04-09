#include "mcv_platform.h"
#include "PropelHighActionState.h"
#include "components/comp_hitbox.h"

PropelHighActionState::PropelHighActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
	animation = "walk";
}

void PropelHighActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	//deltaMovement.y = velocityVector->y * delta;
	PowerStats* currentPowerStats = GetPlayerModel()->GetPowerStats();
	if (timer.elapsed() >= endingTime) {
		if (movementInput != VEC2::Zero) {
			VEC3 inputDirection = GetCamera()->TransformToWorld(movementInput);
			float newYaw = atan2(inputDirection.x, inputDirection.z);
			float y, p, r;
			GetPlayerTransform()->getYawPitchRoll(&y, &p, &r);
			GetPlayerTransform()->setYawPitchRoll(newYaw, p, r);
		}
		velocityVector->y = currentPowerStats->jumpVelocityVector.y;
		deltaMovement = *velocityVector * delta;
		GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
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

void PropelHighActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	SetPose();
	endingTime = endingFrames * (1.f / 60);
	propelTarget = GetPlayerModel()->grabTarget;
	CEntity* targetEntity = propelTarget;
	TCompTransform* targetTransform = targetEntity->get<TCompTransform>();
	movingVelocity = targetTransform->getPosition() + VEC3::Up * 2.f - GetPlayerTransform()->getPosition();
	movingVelocity /= endingTime;
	timer.reset();
	GetPlayerModel()->lastWallEntered = nullptr;
	GetPlayerModel()->lastWallNormal = PxVec3(0, 0, 0);
	GetPlayerModel()->getSkeleton()->executeAction(animation);
}

void PropelHighActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	GetPlayerModel()->baseState->SetPose();
}