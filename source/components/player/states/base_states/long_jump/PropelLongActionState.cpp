#include "mcv_platform.h"
#include "PropelLongActionState.h"
#include "components/comp_hitbox.h"

PropelLongActionState::PropelLongActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
}

void PropelLongActionState::update (float delta) {
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
		*velocityVector = GetPlayerTransform()->getFront() * currentPowerStats->longJumpVelocityVector.z;
		velocityVector->y = currentPowerStats->longJumpVelocityVector.y;
		deltaMovement = *velocityVector * delta;

		GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::AirborneLong);
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
	else {
		*velocityVector = VEC3::Zero;
		deltaMovement = movingVelocity * delta;
	}
}

void PropelLongActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	SetPose();
	endingTime = endingFrames * (1.f / 60);
	propelTarget = GetPlayerModel()->grabTarget;
	CEntity* targetEntity = propelTarget;
	TCompTransform* targetTransform = targetEntity->get<TCompTransform>();
	movingVelocity = targetTransform->getPosition() + VEC3::Up * 2.f - GetPlayerTransform()->getPosition();
	movingVelocity /= endingTime;
	timer.reset();
}

void PropelLongActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	GetPlayerModel()->baseState->SetPose();
}

void PropelLongActionState::OnJumpHighButton() {
}

void PropelLongActionState::OnJumpLongButton() {
}

void PropelLongActionState::OnLanding() {
	//GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::Landing);
	//GetPlayerModel()->SetConcurrentState(TCompPlayerModel::ActionStates::Idle);
}