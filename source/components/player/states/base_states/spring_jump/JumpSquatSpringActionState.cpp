#include "mcv_platform.h"
#include "JumpSquatSpringActionState.h"
#include "components/player/comp_player_model.h"
#include "components/player/states/StateManager.h"

JumpSquatSpringActionState::JumpSquatSpringActionState(StateManager* stateManager) :
	GroundedActionState(stateManager, JumpSquatSpring) {
}

void JumpSquatSpringActionState::update(float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	PowerStats* currentPowerStats = getPlayerModel()->getPowerStats();
	if (timer.elapsed() >= squatTime) {
		//saltar
		//getPlayerModel()->isAttachedToPlatform = false;
		velocityVector->y = currentPowerStats->springJumpVelocity;
		//Dejamos que el cambio de estado se haga cuando lo detecte ground sensor
		deltaMovement = *velocityVector * delta;
		getPlayerModel()->wannaJump = true;
	}
}

void JumpSquatSpringActionState::onStateEnter(IActionState * lastState) {
	GroundedActionState::onStateEnter(lastState);
	timer.reset();
	getPlayerModel()->maxVerticalSpeed = getPlayerModel()->maxVelocityUpwards;
	//getSkeleton()->executeAction(animation, 0.05f, 0.05f);
}

void JumpSquatSpringActionState::onStateExit(IActionState * nextState) {
	GroundedActionState::onStateExit(nextState);
}

void JumpSquatSpringActionState::onLeavingGround() {
	if (timer.elapsed() >= squatTime) {
		timer.reset();
		stateManager->changeState(SpringJump);
	}
	else {
		//En caso de que el comportamiento fuera diferente si cae antes de poder saltar
		stateManager->changeState(GhostJumpSquat);
	}
}

