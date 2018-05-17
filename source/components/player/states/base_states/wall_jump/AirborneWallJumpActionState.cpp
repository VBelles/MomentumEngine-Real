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
	deltaMovement.x = velocityVector->x * delta;
	deltaMovement.y = velocityVector->y * delta;
	deltaMovement.z = velocityVector->z * delta;
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
	//dbg("Entrando en airborne long\n");
	getPlayerModel()->getSkeleton()->blendCycle(animation, 0.2f, 0.2f);
}

void AirborneWallJumpActionState::onStateExit(IActionState * nextState) {
	AirborneActionState::onStateExit(nextState);
	//dbg("Saliendo de airborne long\n");
}

