#include "mcv_platform.h"
#include "JumpSquatActionState.h"

JumpSquatActionState::JumpSquatActionState(TCompPlayerModel * player)
	: GroundedActionState::GroundedActionState(player) {
}


void JumpSquatActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	dbg("Entrando en JumpSquat\n");
	squatTime = squatFrames * (1.f / 60);
	timer.reset();
	enteringVelocity = player->GetVelocityVector()->Length();
}

void JumpSquatActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	dbg("Saliendo de JumpSquat\n");
}

void JumpSquatActionState::update (float delta) {
	if (timer.elapsed() >= squatTime) {
		//saltar
		*player->GetVelocityVector() += player->jumpVelocityVector;
		//Dejamos que el cambio de estado se haga cuando lo detecte grouns sensor

	}
}

void JumpSquatActionState::SetMovementInput(VEC2 input, float delta) {
	bool hasInput = input != VEC2::Zero;
	playerTransform = player->GetTransform();
	currentCamera = player->GetCamera();
	collider = player->GetCollider();
	accelerationVector = player->GetAccelerationVector();
	velocityVector = player->GetVelocityVector();

	deltaMovement.x = 0;
	deltaMovement.z = 0;
	if (hasInput) {
		//usar entering velocity en vez de player->speedFactor (una vez el movimiento se haga con velocityVector)
		deltaMovement = playerTransform->getFront() * player->speedFactor * delta;
	}

	player->currentGravityMultiplier = velocityVector->y < 0 ? player->fallingMultiplier : player->normalGravityMultiplier;
	deltaMovement.y = velocityVector->y * delta + 0.5f * accelerationVector->y * player->currentGravityMultiplier * delta * delta;
	physx::PxControllerCollisionFlags myFlags = collider->controller->move(physx::PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, delta, physx::PxControllerFilters());
	velocityVector->y += clamp(accelerationVector->y * delta, -player->maxVelocityVertical, player->maxVelocityVertical);

	player->isGrounded = myFlags.isSet(physx::PxControllerCollisionFlag::Enum::eCOLLISION_DOWN);
	if (player->isGrounded) {
		velocityVector->y = 0;
	}
	else {
		OnLeavingGround();
	}
}

//ni caso a este input
void JumpSquatActionState::OnJumpHighButton() {}

void JumpSquatActionState::OnLeavingGround() {
	if (timer.elapsed() >= squatTime) {
		timer.reset();
		player->SetActionState(TCompPlayerModel::ActionStates::Airborne);
	}
	else {
		//En caso de que el comportamiento fuera diferente si cae antes de poder saltar
		player->SetActionState(TCompPlayerModel::ActionStates::Airborne);
	}
}
