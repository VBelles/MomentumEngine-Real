#include "mcv_platform.h"
#include "GhostJumpSquatActionState.h"

GhostJumpSquatActionState::GhostJumpSquatActionState(TCompPlayerModel * player)
	: AirborneActionState::AirborneActionState(player) {
}


void GhostJumpSquatActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	dbg("Entrando en GhostJumpSquat\n");
	squatTime = squatFrames * (1.f / 60);
	timer.reset();
	enteringVelocity = player->GetVelocityVector()->Length();
}

void GhostJumpSquatActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	dbg("Saliendo de GhostJumpSquat\n");
}

void GhostJumpSquatActionState::update (float delta) {
	if (timer.elapsed() >= squatTime) {
		//saltar
		PowerStats* currentPowerStats = player->GetPowerStats();
		VEC3* velocityVector = player->GetVelocityVector();
		velocityVector->y = 0.f;
		*velocityVector += currentPowerStats->jumpVelocityVector;
		//Como estamos ya en el aire, hacemos el cambio nosotros mismos
		player->SetActionState(TCompPlayerModel::ActionStates::Airborne);
	}
}

void GhostJumpSquatActionState::SetMovementInput(VEC2 input, float delta) {
	bool hasInput = input != VEC2::Zero;
	playerTransform = player->GetTransform();
	currentCamera = player->GetCamera();
	collider = player->GetCollider();
	accelerationVector = player->GetAccelerationVector();
	velocityVector = player->GetVelocityVector();
	PowerStats* currentPowerStats = player->GetPowerStats();

	deltaMovement.x = 0;
	deltaMovement.z = 0;
	if (hasInput) {
		//usar entering velocity en vez de player->maxHorizontalSpeed (una vez el movimiento se haga con velocityVector)
		deltaMovement = playerTransform->getFront() * currentPowerStats->maxHorizontalSpeed * delta;
	}

	currentPowerStats->currentGravityMultiplier = velocityVector->y < 0 ? currentPowerStats->fallingMultiplier : currentPowerStats->normalGravityMultiplier;
	deltaMovement.y = velocityVector->y * delta + 0.5f * accelerationVector->y * currentPowerStats->currentGravityMultiplier * delta * delta;
	physx::PxControllerCollisionFlags myFlags = collider->controller->move(physx::PxVec3(deltaMovement.x, deltaMovement.y, deltaMovement.z), 0.f, delta, physx::PxControllerFilters());
	velocityVector->y += clamp(accelerationVector->y * delta, -currentPowerStats->maxVelocityVertical, currentPowerStats->maxVelocityVertical);

	player->isGrounded = myFlags.isSet(physx::PxControllerCollisionFlag::Enum::eCOLLISION_DOWN);
	if (player->isGrounded) {
		OnLanding();
	}
}

//ni caso a este input
void GhostJumpSquatActionState::OnJumpHighButton() {}

void GhostJumpSquatActionState::OnLanding() {
	//Ir a landing action state
	player->SetActionState(TCompPlayerModel::ActionStates::Grounded);
}
