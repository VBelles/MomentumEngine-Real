#include "mcv_platform.h"
#include "components/player/comp_player_model.h"
#include "TurnAroundActionState.h"
#include "components/comp_camera.h"
#include "components/comp_transform.h"

TurnAroundActionState::TurnAroundActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
}

void TurnAroundActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	if (timer.elapsed() >= turnAroundTime) {
		rotateToFinalDirection();
		if (!isChangingBaseState) {
			if (movementInput.Length() > 0.8f) {
				setFinalVelocity();
				getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Walk);
			}
			else {
				getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Idle);
			}
		}
	}
	else {
		float y, p, r;
		getPlayerTransform()->getYawPitchRoll(&y,&p,&r);
		y += rotationSpeed * delta;
		getPlayerTransform()->setYawPitchRoll(y, p, r);
	}
}

void TurnAroundActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	timer.reset();
	VEC2 enteringHorizontalVelocityVector = { velocityVector->x, velocityVector->z };
	*velocityVector = VEC3::Zero;
	float enteringHorizontalVelocity = enteringHorizontalVelocityVector.Length();
	movementInput = lastState->getMovementInput();
	movementInput.Normalize();
	VEC3 movementInputWorldSpace = getCamera()->TransformToWorld(movementInput);
	exitVelocityVector = movementInputWorldSpace * enteringHorizontalVelocity;
	exitYaw = atan2(movementInputWorldSpace.x, movementInputWorldSpace.z);
	float y, p, r;
	getPlayerTransform()->getYawPitchRoll(&y, &p, &r);
	rotationSpeed = (exitYaw - y) / turnAroundTime;
}

void TurnAroundActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
}

void TurnAroundActionState::onJumpHighButton() {
	//Quizás se puede usar una variable para decir que saldrá en salto y que no salga directamente desde aquí 
	//(que se coma todos los frames si quiere saltar)
	rotateToFinalDirection();
	setFinalVelocity();
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::JumpSquat);
}

void TurnAroundActionState::onJumpLongButton() {
	rotateToFinalDirection();
	setFinalVelocity();
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::JumpSquatLong);
}

void TurnAroundActionState::onLeavingGround() {
	rotateToFinalDirection();
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}

void TurnAroundActionState::rotateToFinalDirection() {
	//Rotar hasta el ángulo de salida
	float y, p, r;
	getPlayerTransform()->getYawPitchRoll(&y, &p, &r);
	getPlayerTransform()->setYawPitchRoll(exitYaw, p, r);
}

void TurnAroundActionState::setFinalVelocity() {
	//Poner velocidad final
	velocityVector->x = exitVelocityVector.x * velocityLossMultiplier;
	velocityVector->z = exitVelocityVector.z * velocityLossMultiplier;
}
