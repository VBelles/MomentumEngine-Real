#include "mcv_platform.h"
#include "GhostJumpSquatActionState.h"

GhostJumpSquatActionState::GhostJumpSquatActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
}

void GhostJumpSquatActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	PowerStats* currentPowerStats = GetPlayerModel()->GetPowerStats();

	if (timer.elapsed() >= squatTime) {
		//saltar
		velocityVector->y = 0.f;
		*velocityVector += currentPowerStats->jumpVelocityVector;
		deltaMovement = *velocityVector * delta;
		//Como estamos ya en el aire, hacemos el cambio nosotros mismos
		GetPlayerModel()->SetBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
	}
	else {
		bool hasInput = movementInput != VEC2::Zero;

		if (hasInput) {
			deltaMovement += GetPlayerTransform()->getFront() * enteringVelocity * delta;
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


