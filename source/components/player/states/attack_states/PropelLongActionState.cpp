#include "mcv_platform.h"
#include "PropelLongActionState.h"
#include "components/comp_hitbox.h"

PropelLongActionState::PropelLongActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
}

void PropelLongActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	PowerStats* currentPowerStats = GetPlayerModel()->GetPowerStats();
	if (timer.elapsed() >= endingTime) {
		if (movementInput != VEC2::Zero) {
			VEC3 inputDirection = GetPlayerModel()->GetCamera()->TransformToWorld(movementInput);
			float newYaw = atan2(inputDirection.x, inputDirection.z);
			float y, p, r;
			GetPlayerTransform()->getYawPitchRoll(&y, &p, &r);
			GetPlayerTransform()->setYawPitchRoll(newYaw, p, r);
		}
		*velocityVector = GetPlayerTransform()->getFront() * currentPowerStats->longJumpVelocityVector.z;
		velocityVector->y = currentPowerStats->longJumpVelocityVector.y;
		deltaMovement = *velocityVector * delta;

		GetPlayerModel()->SetAttackState(TCompPlayerModel::ActionStates::Idle);
		GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::AirborneLong);
		//pasar mensaje a la otra entidad
		CHandle playerEntity = playerModelHandle.getOwner();
		CEntity* targetEntity = propelTarget;
		VEC3 propelVelocity = { 0, -currentPowerStats->jumpVelocityVector.y, 0 };
		targetEntity->sendMsg(TMsgPropelled{ playerEntity, propelVelocity });
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
	GetPlayerModel()->lockMovementState = true;
	GetPlayerModel()->lockWalk = true;
}

void PropelLongActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	GetPlayerModel()->movementState->SetPose();
	GetPlayerModel()->lockMovementState = false;
	GetPlayerModel()->lockWalk = false;
}

void PropelLongActionState::OnJumpHighButton() {
}

void PropelLongActionState::OnJumpLongButton() {
}

void PropelLongActionState::OnLanding() {
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::Landing);
	GetPlayerModel()->SetAttackState(TCompPlayerModel::ActionStates::Idle);
}