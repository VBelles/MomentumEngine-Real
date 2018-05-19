#pragma once

#include "components/player/states/GroundedActionState.h"

class WalkActionState : public GroundedActionState {
private:
	std::string animation = "run";

public:
	WalkActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
};