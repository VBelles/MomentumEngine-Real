#include "mcv_platform.h"
#include "SlideActionState.h"
#include "components/player/comp_player_model.h"
#include "components/player/states/StateManager.h"

SlideActionState::SlideActionState(StateManager* stateManager) :
	AirborneActionState(stateManager, Slide) {
}

void SlideActionState::update(float delta) {
	AirborneActionState::update(delta);
	if (stuckTimer.elapsed() >= STUCK_TIME) {
		float y = getPlayerTransform()->getPosition().y;
		if (abs(y - stuckY) < STUCK_THRESHOLD) {
			canJump = true;
		}
		else {
			stuckY = y;
			canJump = false;
		}
		stuckTimer.reset();
	}
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	if (isJumping) {
		velocityVector->y = 0.f;
		*velocityVector += currentPowerStats->jumpVelocityVector;
		deltaMovement = *velocityVector * delta;
		deltaMovement.y += 0.15f;
		getPlayerModel()->wannaJump = true;
		stateManager->changeState(AirborneNormal);
	}
	if (isLongJumping) {
		*velocityVector = getPlayerTransform()->getFront() * currentPowerStats->longJumpVelocityVector.z;
		velocityVector->y = currentPowerStats->longJumpVelocityVector.y;
		deltaMovement = *velocityVector * delta;
		deltaMovement.y += 0.15f;
		getPlayerModel()->wannaJump = true;
		stateManager->changeState(AirborneLong);
	}
}

void SlideActionState::onStateEnter(IActionState* lastState) {
	AirborneActionState::onStateEnter(lastState);

	TCompTransform* transform = getPlayerTransform();
	float yaw, pitch;
	transform->getYawPitchRoll(&yaw, &pitch);

	getPlayerModel()->maxVerticalSpeed = maxVerticalSlidingVelocity;
	velocityVector->y = clamp(velocityVector->y, -maxVerticalSlidingVelocity, maxVerticalSlidingVelocity);

	getSkeleton()->blendCycle(animation, 0.2f, 0.2f);

	stuckY = transform->getPosition().y;
	stuckTimer.reset();

	canJump = false;
	isJumping = false;
	isLongJumping = false;


}

void SlideActionState::onStateExit(IActionState* nextState) {
	AirborneActionState::onStateExit(nextState);
}

void SlideActionState::onJumpHighButton() {
	if (canJump) {
		isJumping = true;
	}
}

void SlideActionState::onJumpLongButton() {
	if (canJump) {
		isLongJumping = true;
	}
}

void SlideActionState::onMove(MoveState& moveState) {
	if (!moveState.isTouchingBot) { //Not grounded, change to airborne normal
		stateManager->changeState(AirborneNormal);
	}
	else if (isWalkable(moveState)) { //Grounded
		stateManager->changeState(Walk);
	}
}
