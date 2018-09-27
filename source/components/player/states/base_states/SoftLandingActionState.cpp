#include "mcv_platform.h"
#include "SoftLandingActionState.h"
#include "components/player/comp_player_model.h"
#include "components/player/states/StateManager.h"


SoftLandingActionState::SoftLandingActionState(StateManager* stateManager):
	LandingActionState(stateManager, SoftLanding){
}

void SoftLandingActionState::update (float delta) {
	LandingActionState::update(delta);
}

void SoftLandingActionState::onStateEnter(IActionState * lastState) {
	LandingActionState::onStateEnter(lastState);
	landingLagTime = frames2sec(getPlayerModel()->getPowerStats()->landingLag);
	getSkeleton()->executeAction(animation, 0.2f, 0.2f);
}

void SoftLandingActionState::onStateExit(IActionState * nextState) {
	LandingActionState::onStateExit(nextState);
}
