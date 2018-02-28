#include "mcv_platform.h"
#include "PropelLongActionState.h"
#include "components/comp_hitbox.h"

PropelLongActionState::PropelLongActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
}

void PropelLongActionState::update (float delta) {
	PowerStats* currentPowerStats = GetPlayerModel()->GetPowerStats();
	if (timer.elapsed() >= endingTime) {
		velocityVector->y = currentPowerStats->jumpVelocityVector.y;
		deltaMovement = *velocityVector * delta;
		GetPlayerModel()->SetAttackState(TCompPlayerModel::ActionStates::Idle);
		GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::AirborneNormal);
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
	GetPlayerModel()->lockMovementState = false;
	GetPlayerModel()->lockWalk = false;
}

void PropelLongActionState::SetMovementInput(VEC2 input) {
}

void PropelLongActionState::OnJumpHighButton() {
}

void PropelLongActionState::OnJumpLongButton() {
}

void PropelLongActionState::OnLanding() {
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::Run);
	GetPlayerModel()->SetAttackState(TCompPlayerModel::ActionStates::Idle);
}