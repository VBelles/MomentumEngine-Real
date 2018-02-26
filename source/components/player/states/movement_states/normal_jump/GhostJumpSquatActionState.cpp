#include "mcv_platform.h"
#include "GhostJumpSquatActionState.h"

GhostJumpSquatActionState::GhostJumpSquatActionState(TCompPlayerModel * player)
	: AirborneActionState::AirborneActionState(player) {
}


void GhostJumpSquatActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	//dbg("Entrando en GhostJumpSquat\n");
	squatTime = squatFrames * (1.f / 60);
	timer.reset();
	enteringVelocity = player->GetVelocityVector()->Length();
}

void GhostJumpSquatActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	//dbg("Saliendo de GhostJumpSquat\n");
}

void GhostJumpSquatActionState::update (float delta) {
	PowerStats* currentPowerStats = player->GetPowerStats();
	collider = player->GetCollider();

	if (timer.elapsed() >= squatTime) {
		//saltar
		velocityVector->y = 0.f;
		*velocityVector += currentPowerStats->jumpVelocityVector;
		deltaMovement = *velocityVector * delta;
		//Como estamos ya en el aire, hacemos el cambio nosotros mismos
		player->SetMovementState(TCompPlayerModel::ActionStates::AirborneNormal);
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

		//distancia vertical recorrida
		currentPowerStats->currentGravityMultiplier = velocityVector->y < 0 ? currentPowerStats->fallingMultiplier : currentPowerStats->normalGravityMultiplier;
		deltaMovement.y = CalculateVerticalDeltaMovement(delta, accelerationVector->y * currentPowerStats->currentGravityMultiplier, currentPowerStats->maxVelocityVertical);

		//Nueva velocidad vertical y clampeo
		velocityVector->y += accelerationVector->y * currentPowerStats->currentGravityMultiplier * delta;
	}

	velocityVector->y = clamp(velocityVector->y, -currentPowerStats->maxVelocityVertical, currentPowerStats->maxVelocityVertical);
}

void GhostJumpSquatActionState::SetMovementInput(VEC2 input) {
	movementInput = input;
}

//ni caso a este input
void GhostJumpSquatActionState::OnJumpHighButton() {}

void GhostJumpSquatActionState::OnLanding() {
	//Ir a landing action state
	player->SetMovementState(TCompPlayerModel::ActionStates::Run);
}
