#include "mcv_platform.h"
#include "TurnAroundActionState.h"


TurnAroundActionState::TurnAroundActionState(CHandle playerHandle)
	: GroundedActionState::GroundedActionState(playerHandle) {
}

void TurnAroundActionState::update (float delta) {
	GroundedActionState::update(delta);
	float y, p, r;
	GetPlayerTransform()->getYawPitchRoll(&y,&p,&r);
	y *= rotationSpeed * delta;
	GetPlayerTransform()->setYawPitchRoll(y, p, r);
}

void TurnAroundActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	turnAroundTime = turnAroundFrames * (1 / 60.f);
	timer.reset();
	VEC2 enteringHorizontalVelocityVector = { velocityVector->x, velocityVector->z };
	*velocityVector = VEC3::Zero;
	float enteringHorizontalVelocity = enteringHorizontalVelocityVector.Length();
	movementInput = lastState->GetMovementInput();
	movementInput.Normalize();
	exitVelocityVector = VEC3::Zero;
	exitVelocityVector.x = movementInput.x * enteringHorizontalVelocity;
	exitVelocityVector.z = movementInput.y * enteringHorizontalVelocity;
	rotationSpeed = GetPlayerTransform()->getDeltaYawToAimTo(GetPlayer()->GetCamera()->TransformToWorld(exitVelocityVector));
	rotationSpeed /= turnAroundTime;
	//dbg("Entrando en turn around\n");
}

void TurnAroundActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	//poner velocidad a la misma de entrada, en la dirección de movementInput
	//dbg("Saliendo de turn around\n");
}

void TurnAroundActionState::SetMovementInput(VEC2 input) {
	//movementInput = input;
}

void TurnAroundActionState::OnJumpHighButton() {
	RotateToFinalDirection();
	SetFinalVelocity();
	GetPlayer()->SetMovementState(TCompPlayerModel::ActionStates::JumpSquat);
}

void TurnAroundActionState::OnJumpLongButton() {
	RotateToFinalDirection();
	SetFinalVelocity();
	GetPlayer()->SetMovementState(TCompPlayerModel::ActionStates::JumpSquatLong);
}

void TurnAroundActionState::OnLeavingGround() {
	RotateToFinalDirection();
	GetPlayer()->SetMovementState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}

void TurnAroundActionState::RotateToFinalDirection() {
	//Rotar hasta el ángulo de salida
	float newYaw = GetPlayerTransform()->getDeltaYawToAimTo(GetPlayer()->GetCamera()->TransformToWorld(exitVelocityVector));
	float y, p, r;
	GetPlayerTransform()->getYawPitchRoll(&y, &p, &r);
	GetPlayerTransform()->setYawPitchRoll(newYaw, p, r);
}

void TurnAroundActionState::SetFinalVelocity() {
	//Poner velocidad final
	velocityVector->x = exitVelocityVector.x;
	velocityVector->z = exitVelocityVector.z;
}
