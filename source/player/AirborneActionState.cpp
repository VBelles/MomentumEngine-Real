#include "mcv_platform.h"
#include "AirborneActionState.h"

AirborneActionState::AirborneActionState(TCompPlayerModel * player) 
	: IActionState::IActionState(player) {
}

void AirborneActionState::update (float delta) {
}

void AirborneActionState::OnStateEnter(IActionState * lastState) {
	IActionState::OnStateEnter(lastState);
	dbg("Entrando en airborne\n");
}

void AirborneActionState::OnStateExit(IActionState * nextState) {
	IActionState::OnStateExit(nextState);
	dbg("Saliendo de airborne\n");
}

void AirborneActionState::SetMovementInput(VEC2 input, float delta) {
	bool hasInput = input != VEC2::Zero;
	playerTransform = player->GetTransform();
	currentCamera = player->GetCamera();
	collider = player->GetCollider();
	accelerationVector = player->GetAccelerationVector();
	velocityVector = player->GetVelocityVector();
	PowerStats* currentPowerStats = player->GetPowerStats();
	float acceleration = player->GetAcceleration();

	VEC3 desiredDirection = player->GetCamera()->TransformToWorld(input);
	VEC3 targetPos = playerTransform->getPosition() + desiredDirection * currentPowerStats->maxHorizontalSpeed * delta;

	if (hasInput && abs(playerTransform->getDeltaYawToAimTo(targetPos)) > 0.01f) {
		float y, p, r;
		playerTransform->getYawPitchRoll(&y, &p, &r);
		float yMult = playerTransform->isInLeft(targetPos) ? 1.f : -1.f;
		y += currentPowerStats->rotationSpeed * delta * yMult;
		playerTransform->setYawPitchRoll(y, p, r);
	}

	deltaMovement.x = 0;
	deltaMovement.z = 0;
	VEC2 horizontalVelocity = { velocityVector->x , velocityVector->z };
	float currentSpeed = horizontalVelocity.Length();
	//dbg("speed: %f\n", currentSpeed);
	if (hasInput) {
		VEC3 resultingDirection = *velocityVector;
		resultingDirection.y = 0.f;
		resultingDirection += desiredDirection * acceleration * delta;
		deltaMovement = resultingDirection * delta;
		//deltaMovement = desiredDirection * currentSpeed * delta + 0.5f * desiredDirection * acceleration * delta * delta;

		horizontalVelocity.x += desiredDirection.x * currentSpeed;
		horizontalVelocity.y += desiredDirection.z * currentSpeed;

		velocityVector->x = horizontalVelocity.x + desiredDirection.x * acceleration * delta;
		velocityVector->z = horizontalVelocity.y + desiredDirection.z * acceleration * delta;

		//clampear velocidad horizontal
		horizontalVelocity.x = velocityVector->x;
		horizontalVelocity.y = velocityVector->z;

		if (horizontalVelocity.Length() > currentPowerStats->maxHorizontalSpeed) {
			horizontalVelocity.Normalize();
			horizontalVelocity *= currentPowerStats->maxHorizontalSpeed;
			velocityVector->x = horizontalVelocity.x;
			velocityVector->z = horizontalVelocity.y;
		}
	}
	else {
		deltaMovement.x = velocityVector->x * delta;
		deltaMovement.z = velocityVector->z * delta;
	}

	currentPowerStats->currentGravityMultiplier = velocityVector->y < 0 ? currentPowerStats->fallingMultiplier : currentPowerStats->normalGravityMultiplier;
	deltaMovement.y = velocityVector->y * delta + 0.5f * accelerationVector->y * currentPowerStats->currentGravityMultiplier * delta * delta;
	physx::PxControllerCollisionFlags myFlags = collider->controller->move(physx::PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, delta, physx::PxControllerFilters());

	velocityVector->y += accelerationVector->y * delta;
	velocityVector->y = clamp(velocityVector->y, -currentPowerStats->maxVelocityVertical, currentPowerStats->maxVelocityVertical);

	player->isGrounded = myFlags.isSet(physx::PxControllerCollisionFlag::Enum::eCOLLISION_DOWN);
	if (player->isGrounded) {
		isTouching = false;
		OnLanding();
	}
	if (!isTouching) {
		isTouching = myFlags.isSet(physx::PxControllerCollisionFlag::Enum::eCOLLISION_UP);
		if (isTouching) {
			velocityVector->y = 0.f;
		}
	}
}

void AirborneActionState::OnJumpHighButton() {
	//grab
}

void AirborneActionState::OnLanding() {
	//Ir a landing action state
	player->SetActionState(TCompPlayerModel::ActionStates::Grounded);
}
