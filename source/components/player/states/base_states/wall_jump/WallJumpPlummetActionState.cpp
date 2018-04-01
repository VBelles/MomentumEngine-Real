#include "mcv_platform.h"
#include "WallJumpPlummetActionState.h"

WallJumpPlummetActionState::WallJumpPlummetActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
}

void WallJumpPlummetActionState::update (float delta) {
	deltaMovement = VEC3::Zero;

	VEC2 horizontalVelocity = { velocityVector->x, velocityVector->z };
	if (deceleration * delta < horizontalVelocity.Length()) {
		deltaMovement = CalculateHorizontalDeltaMovement(delta, VEC3(velocityVector->x, 0, velocityVector->z),
			-VEC3(velocityVector->x, 0, velocityVector->z), deceleration, 30.f);

		TransferVelocityToDirectionAndAccelerate(delta, false, -VEC3(velocityVector->x, 0, velocityVector->z), deceleration);
	}

	deltaMovement = *velocityVector * delta;
	if (timer.elapsed() >= endingTime) {
		GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	}
}

void WallJumpPlummetActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	GetPlayerModel()->SetGravityMultiplier(0.f);
	SetPose();
	//Se calcula cada vez que se entra, ya que depende del poder
	endingTime = endingFrames * (1.f / 60);
	timer.reset();
}

void WallJumpPlummetActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
}

void WallJumpPlummetActionState::OnJumpHighButton() {
	//Se recupera y agarra
	GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	GetPlayerModel()->SetConcurrentState(TCompPlayerModel::ActionStates::GrabHigh);
}

void WallJumpPlummetActionState::OnJumpLongButton() {
	GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	GetPlayerModel()->SetConcurrentState(TCompPlayerModel::ActionStates::GrabLong);
}

void WallJumpPlummetActionState::OnFastAttackButton() {
	//Se recupera
	GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
}

void WallJumpPlummetActionState::OnStrongAttackButton() {
	GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
}

void WallJumpPlummetActionState::OnLanding() {
	GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::LandingFallingAttack);
}
