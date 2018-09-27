#include "mcv_platform.h"
#include "WallJumpSquatActionState.h"
#include "components/player/comp_player_model.h"
#include "components/player/states/StateManager.h"


WallJumpSquatActionState::WallJumpSquatActionState(StateManager* stateManager) :
	AirborneActionState(stateManager, WallJumpSquat) {
}

void WallJumpSquatActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	//deltaMovement.y = velocityVector->y * delta;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();

	if (!stateManager->isChangingBaseState) {
		if (timer.elapsed() >= endingTime) {
			//saltar
			*velocityVector = getPlayerTransform()->getFront() * currentPowerStats->wallJumpVelocityVector.z;
			velocityVector->y = currentPowerStats->wallJumpVelocityVector.y;

			deltaMovement = *velocityVector * delta;
			getSkeleton()->executeAction(animation, 0.1f, 0.1f);
			stateManager->changeState(AirborneWallJump); 
		}
	}
}

void WallJumpSquatActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	getSkeleton()->blendCycle(animationHugging, 0.1f, 0.1f);
	timer.reset();
	EngineSound.emitEvent(SOUND_JUMP);
}

void WallJumpSquatActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
}
