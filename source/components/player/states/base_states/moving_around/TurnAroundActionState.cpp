#include "mcv_platform.h"
#include "components/player/comp_player_model.h"
#include "TurnAroundActionState.h"
#include "components/comp_camera.h"
#include "components/comp_transform.h"
#include "components/player/states/StateManager.h"


TurnAroundActionState::TurnAroundActionState(StateManager* stateManager)
	: GroundedActionState::GroundedActionState(stateManager, TurnAround) {
}

void TurnAroundActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	if (timer.elapsed() >= turnAroundTime) {
		rotateToFinalDirection();
		if (!stateManager->isChangingBaseState) {
			if (movementInput.Length() > 0.8f) {
				setFinalVelocity();
				stateManager->changeState(Walk);
			}
			else {
				stateManager->changeState(Idle);
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
	VEC3 movementInputWorldSpace = getCamera()->getCamera()->TransformToWorld(movementInput);
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
	stateManager->changeState(JumpSquat);
}

void TurnAroundActionState::onJumpLongButton() {
	rotateToFinalDirection();
	setFinalVelocity();
	stateManager->changeState(JumpSquatLong);
}

void TurnAroundActionState::onLeavingGround() {
	rotateToFinalDirection();
	stateManager->changeState(GhostJumpWindow);
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
