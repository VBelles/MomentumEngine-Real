#include "mcv_platform.h"
#include "GhostJumpSquatLongActionState.h"

GhostJumpSquatLongActionState::GhostJumpSquatLongActionState(CHandle playerHandle)
	: AirborneActionState::AirborneActionState(playerHandle) {
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
	PowerStats* currentPowerStats = GetPlayer()->GetPowerStats();

	if (timer.elapsed() >= squatTime) {
		//saltar
		*velocityVector = GetPlayerTransform()->getFront() * currentPowerStats->longJumpVelocityVector.z;
		velocityVector->y = currentPowerStats->longJumpVelocityVector.y;
		deltaMovement = *velocityVector * delta;
		//Como estamos ya en el aire, hacemos el cambio nosotros mismos
		GetPlayer()->SetMovementState(TCompPlayerModel::ActionStates::AirborneLong);
	}
	else {
		bool hasInput = movementInput != VEC2::Zero;

		deltaMovement.x = 0;
		deltaMovement.z = 0;
		if (hasInput) {
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

void GhostJumpSquatLongActionState::SetMovementInput(VEC2 input) {
	movementInput = input;
}

//ni caso a este input
void GhostJumpSquatLongActionState::OnJumpHighButton() {}

void GhostJumpSquatLongActionState::OnJumpLongButton() {}

void GhostJumpSquatLongActionState::OnLanding() {
	//Ir a landing action state
	GetPlayer()->SetMovementState(TCompPlayerModel::ActionStates::Run);
}
