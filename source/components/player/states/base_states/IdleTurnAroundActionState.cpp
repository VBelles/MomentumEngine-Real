#include "mcv_platform.h"
#include "IdleTurnAroundActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"

IdleTurnAroundActionState::IdleTurnAroundActionState(CHandle playerModelHandle)
	: GroundedActionState::GroundedActionState(playerModelHandle) {
}

void IdleTurnAroundActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	if (timer.elapsed() >= turnAroundTime) {
		rotateToFinalDirection();
		if (!isChangingBaseState) {
			getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Idle);
		}
	}
	else {
		float y, p, r;
		getPlayerTransform()->getYawPitchRoll(&y,&p,&r);
		y += rotationSpeed * delta;
		getPlayerTransform()->setYawPitchRoll(y, p, r);
	}
}

void IdleTurnAroundActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	turnAroundTime = turnAroundFrames * (1.f / 60);
	timer.reset();
	*velocityVector = VEC3::Zero;
	movementInput = lastState->getMovementInput();
	movementInput.Normalize();
	VEC3 movementInputWorldSpace = getCamera()->TransformToWorld(movementInput);
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
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::JumpSquat);
}

void IdleTurnAroundActionState::onJumpLongButton() {
	rotateToFinalDirection();
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::JumpSquatLong);
}

void IdleTurnAroundActionState::onLeavingGround() {
	rotateToFinalDirection();
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::GhostJumpWindow);
}

void IdleTurnAroundActionState::rotateToFinalDirection() {
	//Rotar hasta el �ngulo de salida
	float y, p, r;
	getPlayerTransform()->getYawPitchRoll(&y, &p, &r);
	getPlayerTransform()->setYawPitchRoll(exitYaw, p, r);
}
