#include "mcv_platform.h"
#include "GhostJumpSquatActionState.h"

GhostJumpSquatActionState::GhostJumpSquatActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
}


void GhostJumpSquatActionState::OnStateEnter(IActionState * lastState) {
	AirborneActionState::OnStateEnter(lastState);
	//dbg("Entrando en GhostJumpSquat\n");
	squatTime = squatFrames * (1.f / 60);
	timer.reset();
	enteringVelocity = GetPlayerModel()->GetVelocityVector()->Length();
}

void GhostJumpSquatActionState::OnStateExit(IActionState * nextState) {
	AirborneActionState::OnStateExit(nextState);
	//dbg("Saliendo de GhostJumpSquat\n");
}

void GhostJumpSquatActionState::update (float delta) {
	PowerStats* currentPowerStats = GetPlayerModel()->GetPowerStats();

	if (timer.elapsed() >= squatTime) {
		//saltar
		velocityVector->y = 0.f;
		*velocityVector += currentPowerStats->jumpVelocityVector;
		deltaMovement = *velocityVector * delta;
		//Como estamos ya en el aire, hacemos el cambio nosotros mismos
		GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::AirborneNormal);
	}
	else {
		bool hasInput = movementInput != VEC2::Zero;

		deltaMovement.x = 0;
		deltaMovement.z = 0;
		if (hasInput) {
			//usar entering velocity en vez de GetPlayerModel()->maxHorizontalSpeed (una vez el movimiento se haga con velocityVector)
			deltaMovement = GetPlayerTransform()->getFront() * enteringVelocity * delta;
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
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::Run);
}
