#include "mcv_platform.h"
#include "AirborneActionState.h"

AirborneActionState::AirborneActionState(TCompPlayerModel * player) 
	: IActionState::IActionState(player) {
}

void AirborneActionState::update (float delta) {
}

void AirborneActionState::OnStateEnter(IActionState * lastState) {
	IActionState::OnStateEnter(lastState);
	//dbg("Entrando en airborne\n");
}

void AirborneActionState::OnStateExit(IActionState * nextState) {
	IActionState::OnStateExit(nextState);
	//dbg("Saliendo de airborne\n");
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
		newVelocity += desiredDirection * acceleration * delta;

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
