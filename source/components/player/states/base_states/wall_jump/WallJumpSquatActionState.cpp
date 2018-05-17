#include "mcv_platform.h"
#include "WallJumpSquatActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"
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
			stateManager->changeState(AirborneWallJump);
		}
	}
}

void WallJumpSquatActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	timer.reset();
	getPlayerModel()->getSkeleton()->executeAction(animation, 0.2f, 0.2f);
}

void WallJumpSquatActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
}