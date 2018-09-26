#include "mcv_platform.h"
#include "LandingHurtActionState.h"
#include "components/player/comp_player_model.h"
#include "components/comp_camera.h"
#include "components/player/states/StateManager.h"


LandingHurtActionState::LandingHurtActionState(StateManager* stateManager):
	LandingActionState(stateManager, LandingHurt){
}

void LandingHurtActionState::update (float delta) {
	deltaMovement = VEC3::Zero;
	deltaMovement.y = velocityVector->y * delta;
	if (!stateManager->isChangingBaseState) {
		if (timer.elapsed() >= landingLagTime) {
			stateManager->changeState(Idle);
		}
	}
}

void LandingHurtActionState::onStateEnter(IActionState * lastState) {
	LandingActionState::onStateEnter(lastState);
	getSkeleton()->executeAction(animation, 0.05f, 0.05f);
}

void LandingHurtActionState::onStateExit(IActionState * nextState) {
	LandingActionState::onStateExit(nextState);
	getSkeleton()->removeAction(animation, 0.4f);
}

void LandingHurtActionState::setRemainingTime(float time) {
	landingLagTime = time;
}
