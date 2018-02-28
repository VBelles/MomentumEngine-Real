#include "mcv_platform.h"
#include "PropelHighActionState.h"
#include "components/comp_hitbox.h"

PropelHighActionState::PropelHighActionState(CHandle playerHandle)
	: AirborneActionState::AirborneActionState(playerHandle) {
}

void PropelHighActionState::update (float delta) {
	PowerStats* currentPowerStats = GetPlayer()->GetPowerStats();
	if (timer.elapsed() >= endingTime) {
		/*float newYaw = GetPlayerTransform()->getDeltaYawToAimTo();
		float y, p, r;
		GetPlayerTransform()->getYawPitchRoll(&y, &p, &r);
		GetPlayerTransform()->setYawPitchRoll(newYaw, p, r);*/
		velocityVector->y = currentPowerStats->jumpVelocityVector.y;
		deltaMovement = *velocityVector * delta;
		GetPlayer()->SetAttackState(TCompPlayerModel::ActionStates::Idle);
		GetPlayer()->SetMovementState(TCompPlayerModel::ActionStates::AirborneNormal);
		//pasar mensaje a la otra entidad
		CHandle playerEntity = CHandle(playerHandle).getOwner();
		CEntity* targetEntity = propelTarget;
		VEC3 propelVelocity = { 0, -currentPowerStats->jumpVelocityVector.y, 0 };
		targetEntity->sendMsg(TMsgPropelled{ playerEntity, propelVelocity });
	}
	else {
		*velocityVector = VEC3::Zero;
		deltaMovement = movingVelocity * delta;
	}
}

void PropelHighActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	endingTime = endingFrames * (1.f / 60);
	propelTarget = GetPlayer()->grabTarget;
	CEntity* targetEntity = propelTarget;
	TCompTransform* targetTransform = targetEntity->get<TCompTransform>();
	movingVelocity = targetTransform->getPosition() + VEC3::Up * 2.f - GetPlayerTransform()->getPosition();
	movingVelocity /= endingTime;
	timer.reset();
	GetPlayer()->lockMovementState = true;
	GetPlayer()->lockWalk = true;
}

void PropelHighActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	GetPlayer()->lockMovementState = false;
	GetPlayer()->lockWalk = false;
}

void PropelHighActionState::SetMovementInput(VEC2 input) {
	movementInput = input;
}

void PropelHighActionState::OnJumpHighButton() {
}

void PropelHighActionState::OnJumpLongButton() {
}

void PropelHighActionState::OnLanding() {
	GetPlayer()->SetMovementState(TCompPlayerModel::ActionStates::Run);
	GetPlayer()->SetAttackState(TCompPlayerModel::ActionStates::Idle);
}