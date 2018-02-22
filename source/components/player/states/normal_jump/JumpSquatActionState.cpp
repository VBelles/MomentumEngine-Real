#include "mcv_platform.h"
#include "JumpSquatActionState.h"

JumpSquatActionState::JumpSquatActionState(TCompPlayerModel * player)
	: GroundedActionState::GroundedActionState(player) {
}


void JumpSquatActionState::OnStateEnter(IActionState * lastState) {
	GroundedActionState::OnStateEnter(lastState);
	//dbg("Entrando en JumpSquat\n");
	squatTime = squatFrames * (1.f / 60);
	timer.reset();
	enteringVelocity = player->GetVelocityVector()->Length();
}

void JumpSquatActionState::OnStateExit(IActionState * nextState) {
	GroundedActionState::OnStateExit(nextState);
	//dbg("Saliendo de JumpSquat\n");
}

void JumpSquatActionState::update (float delta) {
	
	PowerStats* currentPowerStats = player->GetPowerStats();
	velocityVector = player->GetVelocityVector();
	accelerationVector = player->GetAccelerationVector();
	collider = player->GetCollider();

	if (timer.elapsed() >= squatTime) {
		//saltar
		*velocityVector += currentPowerStats->jumpVelocityVector;
		//Dejamos que el cambio de estado se haga cuando lo detecte ground sensor
		deltaMovement = *velocityVector * delta;
	}
	else {
		bool hasInput = movementInput != VEC2::Zero;
		playerTransform = player->GetTransform();

		deltaMovement.x = 0;
		deltaMovement.z = 0;
		if (hasInput) {
			//usar entering velocity en vez de player->maxHorizontalSpeed (una vez el movimiento se haga con velocityVector)
			deltaMovement = playerTransform->getFront() * enteringVelocity * delta;
		}
		currentPowerStats->currentGravityMultiplier = velocityVector->y < 0 ? currentPowerStats->fallingMultiplier : currentPowerStats->normalGravityMultiplier;
		deltaMovement.y = velocityVector->y * delta + 0.5f * accelerationVector->y * currentPowerStats->currentGravityMultiplier * delta * delta;
	}

	physx::PxControllerCollisionFlags myFlags = collider->controller->move(physx::PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, delta, physx::PxControllerFilters());
	velocityVector->y += clamp(accelerationVector->y * delta, -currentPowerStats->maxVelocityVertical, currentPowerStats->maxVelocityVertical);

	player->isGrounded = myFlags.isSet(physx::PxControllerCollisionFlag::Enum::eCOLLISION_DOWN);
	if (player->isGrounded) {
		velocityVector->y = 0;
	}
	else {
		OnLeavingGround();
	}
}

void JumpSquatActionState::SetMovementInput(VEC2 input) {
	movementInput = input;
}

//ni caso a este input
void JumpSquatActionState::OnJumpHighButton() {}

void JumpSquatActionState::OnLeavingGround() {
	if (timer.elapsed() >= squatTime) {
		timer.reset();
		player->SetActionState(TCompPlayerModel::ActionStates::AirborneNormal);
	}
	else {
		//En caso de que el comportamiento fuera diferente si cae antes de poder saltar
		player->SetActionState(TCompPlayerModel::ActionStates::GhostJumpSquat);
	}
}
