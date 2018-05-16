#include "mcv_platform.h"
#include "GhostJumpSquatActionState.h"
#include "components/comp_transform.h"
#include "components/player/comp_player_model.h"


GhostJumpSquatActionState::GhostJumpSquatActionState(StateManager* stateManager) :
	AirborneActionState(stateManager, GhostJumpSquat) {
}

void GhostJumpSquatActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();

	if (timer.elapsed() >= squatTime) {
		if (!isChangingBaseState) {
			//saltar
			velocityVector->y = 0.f;
			*velocityVector += currentPowerStats->jumpVelocityVector;
			deltaMovement = *velocityVector * delta;
			//Como estamos ya en el aire, hacemos el cambio nosotros mismos
			getPlayerModel()->setBaseState(TCompPlayerModel::ActionStates::AirborneNormal);
		}
	}
	else {
		bool hasInput = movementInput != VEC2::Zero;

		if (hasInput) {
			deltaMovement += getPlayerTransform()->getFront() * enteringVelocity * delta;
		}
	}
}

void GhostJumpSquatActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	//dbg("Entrando en GhostJumpSquat\n");
	timer.reset();
	enteringVelocity = getPlayerModel()->getVelocityVector()->Length();
}

void GhostJumpSquatActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
	//dbg("Saliendo de GhostJumpSquat\n");
}


