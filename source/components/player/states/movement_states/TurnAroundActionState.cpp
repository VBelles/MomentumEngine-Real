#include "mcv_platform.h"
#include "TurnAroundActionState.h"


TurnAroundActionState::TurnAroundActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
}

void TurnAroundActionState::update (float delta) {
	dbg("UPDATE turn around: %d\n", GetPlayerModel()->frame);
	if (timer.elapsed() >= turnAroundTime) {
		RotateToFinalDirection();
		SetFinalVelocity();
		GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::Run);
	}
	else {
		float y, p, r;
		GetPlayerTransform()->getYawPitchRoll(&y,&p,&r);
		y += rotationSpeed * delta;
		GetPlayerTransform()->setYawPitchRoll(y, p, r);
	}
}

void TurnAroundActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	turnAroundTime = turnAroundFrames * (1.f / 60);
	timer.reset();
	VEC2 enteringHorizontalVelocityVector = { velocityVector->x, velocityVector->z };
	*velocityVector = VEC3::Zero;
	float enteringHorizontalVelocity = enteringHorizontalVelocityVector.Length();
	movementInput = lastState->GetMovementInput();
	movementInput.Normalize();
	VEC3 movementInputWorldSpace = GetPlayerModel()->GetCamera()->TransformToWorld(movementInput);
	exitVelocityVector = movementInputWorldSpace * enteringHorizontalVelocity;
	exitYaw = atan2(movementInputWorldSpace.x, movementInputWorldSpace.z);
	float y, p, r;
	GetPlayerTransform()->getYawPitchRoll(&y, &p, &r);
	rotationSpeed = (exitYaw - y) / turnAroundTime;
	dbg("Entrando en turn around: %d\n", GetPlayerModel()->frame);
	dbg("enteringVelocity: %f, movementInput: %f, %f\n", enteringHorizontalVelocity, movementInput.x, movementInput.y);
}

void TurnAroundActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	//poner velocidad a la misma de entrada, en la dirección de movementInput
	dbg("Saliendo de turn around: %d\n", GetPlayerModel()->frame);
}

void TurnAroundActionState::SetMovementInput(VEC2 input) {
	//movementInput = input;
}

void TurnAroundActionState::OnJumpHighButton() {
	RotateToFinalDirection();
	SetFinalVelocity();
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::JumpSquat);
}

void TurnAroundActionState::OnJumpLongButton() {
	RotateToFinalDirection();
	SetFinalVelocity();
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::JumpSquatLong);
}

void TurnAroundActionState::OnLeavingGround() {
	RotateToFinalDirection();
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}

void TurnAroundActionState::RotateToFinalDirection() {
	//Rotar hasta el ángulo de salida
	float y, p, r;
	GetPlayerTransform()->getYawPitchRoll(&y, &p, &r);
	GetPlayerTransform()->setYawPitchRoll(exitYaw, p, r);
}

void TurnAroundActionState::SetFinalVelocity() {
	//Poner velocidad final
	velocityVector->x = exitVelocityVector.x;
	velocityVector->z = exitVelocityVector.z;
}
