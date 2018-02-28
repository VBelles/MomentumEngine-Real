#include "mcv_platform.h"
#include "TurnAroundActionState.h"


TurnAroundActionState::TurnAroundActionState(TCompPlayerModel * player)
	: GroundedActionState::GroundedActionState(player) {
}

void TurnAroundActionState::update (float delta) {
	GroundedActionState::update(delta);
	float y, p, r;
	playerTransform->getYawPitchRoll(&y,&p,&r);
	y *= rotationSpeed * delta;
	playerTransform->setYawPitchRoll(y, p, r);
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
	rotationSpeed = playerTransform->getDeltaYawToAimTo(currentCamera->TransformToWorld(exitVelocityVector));
	rotationSpeed /= turnAroundTime;
	//dbg("Entrando en turn around\n");
}

void TurnAroundActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	//poner velocidad a la misma de entrada, en la direcci�n de movementInput
	//dbg("Saliendo de turn around\n");
}

void TurnAroundActionState::SetMovementInput(VEC2 input) {
	//movementInput = input;
}

void TurnAroundActionState::OnJumpHighButton() {
	RotateToFinalDirection();
	SetFinalVelocity();
	player->SetMovementState(TCompPlayerModel::ActionStates::JumpSquat);
}

void TurnAroundActionState::OnJumpLongButton() {
	RotateToFinalDirection();
	SetFinalVelocity();
	player->SetMovementState(TCompPlayerModel::ActionStates::JumpSquatLong);
}

void TurnAroundActionState::OnLeavingGround() {
	RotateToFinalDirection();
	player->SetMovementState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}

void TurnAroundActionState::RotateToFinalDirection() {
	//Rotar hasta el �ngulo de salida
	float newYaw = playerTransform->getDeltaYawToAimTo(currentCamera->TransformToWorld(exitVelocityVector));
	float y, p, r;
	playerTransform->getYawPitchRoll(&y, &p, &r);
	playerTransform->setYawPitchRoll(newYaw, p, r);
}

void TurnAroundActionState::SetFinalVelocity() {
	//Poner velocidad final
	velocityVector->x = exitVelocityVector.x;
	velocityVector->z = exitVelocityVector.z;
}
