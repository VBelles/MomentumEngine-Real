#include "mcv_platform.h"
#include "GhostJumpSquatActionState.h"

GhostJumpSquatActionState::GhostJumpSquatActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
}

void GhostJumpSquatActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
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

		if (hasInput) {
			//usar entering velocity en vez de GetPlayerModel()->maxHorizontalSpeed (una vez el movimiento se haga con velocityVector)
			deltaMovement = GetPlayerTransform()->getFront() * enteringVelocity * delta;
		}
	}
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

//ni caso a este input
void GhostJumpSquatActionState::OnJumpHighButton() {}

void GhostJumpSquatActionState::OnLanding() {
	//Ir a landing action state
	GetPlayerModel()->SetMovementState(TCompPlayerModel::ActionStates::Landing);
}
