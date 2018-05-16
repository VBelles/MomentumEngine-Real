#pragma once

#include "components/player/states/GroundedActionState.h"

class IdleActionState : public GroundedActionState {
private:
	std::string animation = "idle";

public:
	IdleActionState(StateManager* stateManager);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
};