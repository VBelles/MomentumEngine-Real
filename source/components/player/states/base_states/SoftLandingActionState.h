#pragma once

#include "components/player/states/base_states/LandingActionState.h"

class SoftLandingActionState : public LandingActionState {
protected:
	std::string animation = "jump_caida";

public:
	SoftLandingActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	
};