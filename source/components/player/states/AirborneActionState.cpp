#include "mcv_platform.h"
#include "AirborneActionState.h"

AirborneActionState::AirborneActionState(TCompPlayerModel * player) 
	: IActionState::IActionState(player) {
	accelerationVector = player->GetAccelerationVector();
	velocityVector = player->GetVelocityVector();
	playerTransform = player->GetTransform();
}

void AirborneActionState::update (float delta) {
	bool hasInput = movementInput != VEC2::Zero;
	currentCamera = player->GetCamera();
	collider = player->GetCollider();
	PowerStats* currentPowerStats = player->GetPowerStats();

	VEC3 desiredDirection = player->GetCamera()->TransformToWorld(movementInput);
	VEC3 targetPos = playerTransform->getPosition() + desiredDirection;

	if (hasInput && abs(playerTransform->getDeltaYawToAimTo(targetPos)) > 0.01f) {
		float y, p, r;
		playerTransform->getYawPitchRoll(&y, &p, &r);
		float yMult = playerTransform->isInLeft(targetPos) ? 1.f : -1.f;
		y += 3.5f * delta * yMult;
		playerTransform->setYawPitchRoll(y, p, r);
	}

	VEC2 horizontalVelocity = { velocityVector->x , velocityVector->z };
	float currentSpeed = horizontalVelocity.Length();
	//dbg("speed: %f\n", currentSpeed);
	if (hasInput) {
		VEC3 newVelocity = *velocityVector;
		newVelocity.y = 0;
		newVelocity += desiredDirection * currentPowerStats->acceleration * delta;

		horizontalVelocity.x = newVelocity.x;
		horizontalVelocity.y = newVelocity.z;

		if (horizontalVelocity.Length() > currentPowerStats->maxHorizontalSpeed) {
			horizontalVelocity.Normalize();
			horizontalVelocity *= currentPowerStats->maxHorizontalSpeed;
			newVelocity.x = horizontalVelocity.x;
			newVelocity.z = horizontalVelocity.y;
		}

		deltaMovement = newVelocity * delta;

		velocityVector->x = newVelocity.x;
		velocityVector->z = newVelocity.z;
	}
	else {
		deltaMovement.x = velocityVector->x * delta;
		deltaMovement.z = velocityVector->z * delta;
	}

	currentPowerStats->currentGravityMultiplier = velocityVector->y < 0 ? currentPowerStats->fallingMultiplier : currentPowerStats->normalGravityMultiplier;
	float verticalVelocityIncrement = accelerationVector->y * currentPowerStats->currentGravityMultiplier * delta;
	deltaMovement.y = velocityVector->y * delta + 0.5f * verticalVelocityIncrement * delta;

	velocityVector->y += verticalVelocityIncrement;
	velocityVector->y = clamp(velocityVector->y, -currentPowerStats->maxVelocityVertical, currentPowerStats->maxVelocityVertical);
}

void AirborneActionState::OnStateEnter(IActionState * lastState) {
	IActionState::OnStateEnter(lastState);
}

void AirborneActionState::OnStateExit(IActionState * nextState) {
	IActionState::OnStateExit(nextState);
}

void AirborneActionState::SetMovementInput(VEC2 input) {
	movementInput = input;
}

void AirborneActionState::OnJumpHighButton() {
	//grab
}

void AirborneActionState::OnLanding() {
	//Ir a landing action state
	player->SetMovementState(TCompPlayerModel::ActionStates::Run);
}
