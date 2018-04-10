#include "mcv_platform.h"
#include "GhostJumpSquatLongActionState.h"

GhostJumpSquatLongActionState::GhostJumpSquatLongActionState(CHandle playerModelHandle)
	: AirborneActionState::AirborneActionState(playerModelHandle) {
}



void GhostJumpSquatLongActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();

	if (timer.elapsed() >= squatTime) {
		//saltar
		*velocityVector = getPlayerTransform()->getFront() * currentPowerStats->longJumpVelocityVector.z;
		velocityVector->y = currentPowerStats->longJumpVelocityVector.y;
		deltaMovement = *velocityVector * delta;
		//Como estamos ya en el aire, hacemos el cambio nosotros mismos
		getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneLong);
	}
	else {
		bool hasInput = movementInput != VEC2::Zero;

		deltaMovement.x = 0;
		deltaMovement.z = 0;
		if (hasInput) {
			deltaMovement += getPlayerTransform()->getFront() * enteringVelocity * delta;
		}
	}
}

void GhostJumpSquatLongActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	//dbg("Entrando en GhostJumpSquat\n");
	squatTime = squatFrames * (1.f / 60);
	timer.reset();
}

void GhostJumpSquatLongActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
	//dbg("Saliendo de GhostJumpSquat\n");
}
