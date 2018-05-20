#include "mcv_platform.h"
#include "SpringJumpActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_render.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"
#include "components/player/states/StateManager.h"


SpringJumpActionState::SpringJumpActionState(StateManager* stateManager) :
	AirborneActionState(stateManager, SpringJump) {
}

void SpringJumpActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement = *velocityVector * delta;
	if (!stateManager->isChangingBaseState) {
		if (velocityVector->y < driftSpeedThreshold) {
			stateManager->changeState(AirborneNormal);
		}
	}
}

void SpringJumpActionState::onStateEnter(IActionState * lastState) {
	AirborneActionState::onStateEnter(lastState);
	getSkeleton()->blendCycle(animation, 0.2f, 0.2f);
}

void SpringJumpActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
}

