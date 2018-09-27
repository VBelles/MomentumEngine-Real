#include "mcv_platform.h"
#include "GhostJumpSquatLongActionState.h"
#include "components/player/comp_player_model.h"
#include "components/player/states/StateManager.h"


GhostJumpSquatLongActionState::GhostJumpSquatLongActionState(StateManager* stateManager) :
	AirborneActionState(stateManager, GhostJumpSquatLong) {
}

void GhostJumpSquatLongActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();

	if (timer.elapsed() >= squatTime) {
		if (!stateManager->isChangingBaseState) {
			//saltar
			*velocityVector = getPlayerTransform()->getFront() * currentPowerStats->longJumpVelocityVector.z;
			velocityVector->y = currentPowerStats->longJumpVelocityVector.y;
			deltaMovement = *velocityVector * delta;
			//Como estamos ya en el aire, hacemos el cambio nosotros mismos
			getSkeleton()->executeAction(animationJump, 0.2f, 0.2f);
			stateManager->changeState(AirborneLong);
		}
	}
	else {
		bool hasInput = movementInput.Length() > PAD_DEAD_ZONE;

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
	timer.reset();
}

void GhostJumpSquatLongActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
	//dbg("Saliendo de GhostJumpSquat\n");
}
