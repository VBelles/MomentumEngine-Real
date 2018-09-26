#include "mcv_platform.h"
#include "AirborneWallJumpActionState.h"
#include "components/player/comp_player_model.h"
#include "components/player/states/StateManager.h"


AirborneWallJumpActionState::AirborneWallJumpActionState(StateManager* stateManager) :
	AirborneActionState(stateManager, AirborneWallJump) {
}

void AirborneWallJumpActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement = *velocityVector * delta;
	if (!stateManager->isChangingBaseState) {
		if (velocityVector->y < 0) {
			stateManager->changeState(AirborneNormal);
		}
	}
}

void AirborneWallJumpActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	getPlayerModel()->maxVerticalSpeed = enteringPowerStats->maxVelocityVertical;
	getPlayerModel()->setGravityMultiplier(1.1f);
	getSkeleton()->blendCycle(animation, 0.1f, 0.1f);
}

void AirborneWallJumpActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
}

