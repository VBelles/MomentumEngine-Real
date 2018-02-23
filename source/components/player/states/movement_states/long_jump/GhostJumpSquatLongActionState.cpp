#include "mcv_platform.h"
#include "GhostJumpSquatLongActionState.h"

GhostJumpSquatLongActionState::GhostJumpSquatLongActionState(TCompPlayerModel * player)
	: AirborneActionState::AirborneActionState(player) {
}


void GhostJumpSquatLongActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	//dbg("Entrando en GhostJumpSquat\n");
	squatTime = squatFrames * (1.f / 60);
	timer.reset();
}

void GhostJumpSquatLongActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	//dbg("Saliendo de GhostJumpSquat\n");
}

void GhostJumpSquatLongActionState::update (float delta) {
	PowerStats* currentPowerStats = player->GetPowerStats();
	velocityVector = player->GetVelocityVector();
	collider = player->GetCollider();
	accelerationVector = player->GetAccelerationVector();

	if (timer.elapsed() >= squatTime) {
		//saltar
		*velocityVector = playerTransform->getFront() * currentPowerStats->longJumpVelocityVector.z;
		velocityVector->y = currentPowerStats->longJumpVelocityVector.y;
		deltaMovement = *velocityVector * delta;
		//Como estamos ya en el aire, hacemos el cambio nosotros mismos
		player->SetActionState(TCompPlayerModel::ActionStates::AirborneLong);
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
		OnLanding();
	}
}

void GhostJumpSquatLongActionState::SetMovementInput(VEC2 input) {
	movementInput = input;
}

//ni caso a este input
void GhostJumpSquatLongActionState::OnJumpHighButton() {}

void GhostJumpSquatLongActionState::OnJumpLongButton() {}

void GhostJumpSquatLongActionState::OnLanding() {
	//Ir a landing action state
	player->SetActionState(TCompPlayerModel::ActionStates::Run);
}
