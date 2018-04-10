#include "mcv_platform.h"
#include "IdleTurnAroundActionState.h"


IdleTurnAroundActionState::IdleTurnAroundActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
}

void IdleTurnAroundActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	if (timer.elapsed() >= turnAroundTime) {
		RotateToFinalDirection();
		GetPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Idle);
	}
	else {
		float y, p, r;
		GetPlayerTransform()->getYawPitchRoll(&y,&p,&r);
		y += rotationSpeed * delta;
		GetPlayerTransform()->setYawPitchRoll(y, p, r);
	}
}

void IdleTurnAroundActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	turnAroundTime = turnAroundFrames * (1.f / 60);
	timer.reset();
	*velocityVector = VEC3::Zero;
	movementInput = lastState->GetMovementInput();
	movementInput.Normalize();
	VEC3 movementInputWorldSpace = GetCamera()->TransformToWorld(movementInput);
	exitYaw = atan2(movementInputWorldSpace.x, movementInputWorldSpace.z);
	float y, p, r;
	GetPlayerTransform()->getYawPitchRoll(&y, &p, &r);
	rotationSpeed = (exitYaw - y) / turnAroundTime;
}

void IdleTurnAroundActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
}

void IdleTurnAroundActionState::OnJumpHighButton() {
	RotateToFinalDirection();
	GetPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::JumpSquat);
}

void IdleTurnAroundActionState::OnJumpLongButton() {
	RotateToFinalDirection();
	GetPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::JumpSquatLong);
}

void IdleTurnAroundActionState::OnLeavingGround() {
	RotateToFinalDirection();
	GetPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}

void IdleTurnAroundActionState::RotateToFinalDirection() {
	//Rotar hasta el ángulo de salida
	float y, p, r;
	GetPlayerTransform()->getYawPitchRoll(&y, &p, &r);
	GetPlayerTransform()->setYawPitchRoll(exitYaw, p, r);
}
