#include "mcv_platform.h"
#include "LandingActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "skeleton/comp_skeleton.h"


void LandingActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	bool hasInput = movementInput != VEC2::Zero;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	VEC3 desiredDirection = getCamera()->TransformToWorld(movementInput);
	if (hasInput) {
		VEC3 targetPos = getPlayerTransform()->getPosition() + desiredDirection;
		rotatePlayerTowards(delta, targetPos, currentPowerStats->rotationSpeed);
	}

	if (!isChangingBaseState) {
		if (timer.elapsed() >= landingLagTime) {
			if (movementInput.Length() < 0.8f || enteringSpeed == 0.f) {
				getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Idle);
			}
			else {
				velocityVector->x = getPlayerTransform()->getFront().x * enteringSpeed * 0.6f;
				velocityVector->z = getPlayerTransform()->getFront().z * enteringSpeed * 0.6f;
				getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::Walk);
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
	//dbg("Entrando en landing\n");
	getPlayerModel()->getSkeleton()->executeAction(animation, 0.2f, 0.2f);
}

void LandingActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
	//dbg("Saliendo de landing\n");
}

void LandingActionState::onJumpHighButton() {
	SetFinalRotationAndVelocity();
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::JumpSquat);
}

void LandingActionState::onJumpLongButton() {
	SetFinalRotationAndVelocity();
	getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::JumpSquatLong);
}

void LandingActionState::SetFinalRotationAndVelocity() {
	bool hasInput = movementInput != VEC2::Zero;
	if (hasInput) {
		movementInput.Normalize();
		VEC3 movementInputWorldSpace = getCamera()->TransformToWorld(movementInput);
		float exitYaw = atan2(movementInputWorldSpace.x, movementInputWorldSpace.z);
		float y, p, r;
		getPlayerTransform()->getYawPitchRoll(&y, &p, &r);
		getPlayerTransform()->setYawPitchRoll(exitYaw, p, r);
		velocityVector->x = getPlayerTransform()->getFront().x * enteringSpeed;
		velocityVector->z = getPlayerTransform()->getFront().z * enteringSpeed;
	}
}