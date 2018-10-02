#include "mcv_platform.h"
#include "IdleTurnAroundActionState.h"


IdleTurnAroundActionState::IdleTurnAroundActionState(StateManager* stateManager) :
	GroundedActionState(stateManager, IdleTurnAround) {
}

void IdleTurnAroundActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	if (timer.elapsed() >= turnAroundTime) {
		rotateToFinalDirection();
		if (!stateManager->isChangingBaseState) {
			stateManager->changeState(Idle);
		}
	}
	else {
		float y, p, r;
		getPlayerTransform()->getYawPitchRoll(&y, &p, &r);
		y += rotationSpeed * delta;
		getPlayerTransform()->setYawPitchRoll(y, p, r);
	}
}

void IdleTurnAroundActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	timer.reset();
	*velocityVector = VEC3::Zero;
	movementInput = lastState->getMovementInput();
	movementInput.Normalize();
	VEC3 movementInputWorldSpace = getCamera()->getCamera()->TransformToWorld(movementInput);
	exitYaw = atan2(movementInputWorldSpace.x, movementInputWorldSpace.z);
	float y, p, r;
	getPlayerTransform()->getYawPitchRoll(&y, &p, &r);
	rotationSpeed = (exitYaw - y) / turnAroundTime;
}

void IdleTurnAroundActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
}

void IdleTurnAroundActionState::onJumpHighButton() {
	rotateToFinalDirection();
	stateManager->changeState(JumpSquat);
}

void IdleTurnAroundActionState::onJumpLongButton() {
	rotateToFinalDirection();
	stateManager->changeState(JumpSquatLong);
}

void IdleTurnAroundActionState::onLeavingGround() {
	rotateToFinalDirection();
	stateManager->changeState(GhostJumpWindow);
}

void IdleTurnAroundActionState::rotateToFinalDirection() {
	//Rotar hasta el ángulo de salida
	float y, p, r;
	getPlayerTransform()->getYawPitchRoll(&y, &p, &r);
	getPlayerTransform()->setYawPitchRoll(exitYaw, p, r);
}
