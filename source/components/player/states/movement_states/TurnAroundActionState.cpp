#include "mcv_platform.h"
#include "TurnAroundActionState.h"


TurnAroundActionState::TurnAroundActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
}

void TurnAroundActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	if (timer.elapsed() >= turnAroundTime) {
		RotateToFinalDirection();
		if (movementInput.Length() > 0.8f) {
			SetFinalVelocity();
			GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::Run);
		}
		else {
			GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::Idle);
		}
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
	//deltaMovement = lastState->GetDeltaMovement();
}

void TurnAroundActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
}

void TurnAroundActionState::OnJumpHighButton() {
	//Quizás se puede usar una variable para decir que saldrá en salto y que no salga directamente desde aquí 
	//(que se coma todos los frames si quiere saltar)
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
	velocityVector->x = exitVelocityVector.x * velocityLossMultiplier;
	velocityVector->z = exitVelocityVector.z * velocityLossMultiplier;
}
