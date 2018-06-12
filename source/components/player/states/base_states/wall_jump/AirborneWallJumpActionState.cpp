#include "mcv_platform.h"
#include "AirborneWallJumpActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"
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
			getSkeleton()->blendCycle(animationLoop, 0.2f, 0.2f);
		}
	}
}

void AirborneWallJumpActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	getPlayerModel()->maxVerticalSpeed = enteringPowerStats->maxVelocityVertical;
	getPlayerModel()->setGravityMultiplier(1.1f);
	getSkeleton()->executeAction(animationJump, 0.2f, 0.2f);
}

void AirborneWallJumpActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
}

