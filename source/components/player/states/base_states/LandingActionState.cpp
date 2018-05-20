#include "mcv_platform.h"
#include "LandingActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"


LandingActionState::LandingActionState(StateManager* stateManager, State state):
	GroundedActionState(stateManager, state){
}

void LandingActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	bool hasInput = movementInput != VEC2::Zero;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	VEC3 desiredDirection = getCamera()->getCamera()->TransformToWorld(movementInput);
	if (hasInput) {
		VEC3 targetPos = getPlayerTransform()->getPosition() + desiredDirection;
		rotatePlayerTowards(delta, targetPos, currentPowerStats->rotationSpeed);
	}

	if (!stateManager->isChangingBaseState) {
		if (timer.elapsed() >= landingLagTime) {
			if (movementInput.Length() < 0.8f || enteringSpeed == 0.f) {
				stateManager->changeState(Idle);
			}
			else {
				velocityVector->x = getPlayerTransform()->getFront().x * enteringSpeed * 0.6f;
				velocityVector->z = getPlayerTransform()->getFront().z * enteringSpeed * 0.6f;
				stateManager->changeState(Walk);
			}
		}
	}
}

void LandingActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	VEC3 enteringVelocityVector = { velocityVector->x, 0.f, velocityVector->z };
	enteringSpeed = enteringVelocityVector.Length();
	velocityVector->x = 0.f;
	velocityVector->z = 0.f;
	landingLagTime = frames2sec(getPlayerModel()->getPowerStats()->landingLag);
	timer.reset();
	getPlayerModel()->getSkeleton()->executeAction(animation, 0.2f, 0.2f);
}

void LandingActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
}

void LandingActionState::onJumpHighButton() {
	SetFinalRotationAndVelocity();
	stateManager->changeState(JumpSquat);
}

void LandingActionState::onJumpLongButton() {
	SetFinalRotationAndVelocity();
	stateManager->changeState(JumpSquatLong);
}

void LandingActionState::SetFinalRotationAndVelocity() {
	bool hasInput = movementInput != VEC2::Zero;
	if (hasInput) {
		movementInput.Normalize();
		VEC3 movementInputWorldSpace = getCamera()->getCamera()->TransformToWorld(movementInput);
		float exitYaw = atan2(movementInputWorldSpace.x, movementInputWorldSpace.z);
		float y, p, r;
		getPlayerTransform()->getYawPitchRoll(&y, &p, &r);
		getPlayerTransform()->setYawPitchRoll(exitYaw, p, r);
		velocityVector->x = getPlayerTransform()->getFront().x * enteringSpeed;
		velocityVector->z = getPlayerTransform()->getFront().z * enteringSpeed;
	}
}