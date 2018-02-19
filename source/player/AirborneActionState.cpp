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

	VEC3 desiredDirection = player->GetCamera()->TransformToWorld(input);
	VEC3 targetPos = playerTransform->getPosition() + desiredDirection * player->speedFactor * delta;

	if (hasInput && abs(playerTransform->getDeltaYawToAimTo(targetPos)) > 0.01f) {
		float y, p, r;
		playerTransform->getYawPitchRoll(&y, &p, &r);
		float yMult = playerTransform->isInLeft(targetPos) ? 1.f : -1.f;
		y += player->rotationSpeed * delta * yMult;
		playerTransform->setYawPitchRoll(y, p, r);
	}

	deltaMovement.x = 0;
	deltaMovement.z = 0;
	if (hasInput) {
		deltaMovement = playerTransform->getFront() * player->speedFactor * delta;
	}

	player->currentGravityMultiplier = velocityVector->y < 0 ? player->fallingMultiplier : player->normalGravityMultiplier;
	deltaMovement.y = velocityVector->y * delta + 0.5f * accelerationVector->y * player->currentGravityMultiplier * delta * delta;
	physx::PxControllerCollisionFlags myFlags = collider->controller->move(physx::PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, delta, physx::PxControllerFilters());
	velocityVector->y += accelerationVector->y * delta;
	velocityVector->y = clamp(velocityVector->y, -player->maxVelocityVertical, player->maxVelocityVertical);


	player->isGrounded = myFlags.isSet(physx::PxControllerCollisionFlag::Enum::eCOLLISION_DOWN);
	if (player->isGrounded) {
		OnLanding();
	}
}

void AirborneActionState::OnJumpHighButton() {
	//grab 
}

void AirborneActionState::OnLanding() {
	//Ir a landing action state
	player->SetActionState(TCompPlayerModel::ActionStates::Grounded);
}
