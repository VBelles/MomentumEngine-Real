#pragma once

#include "components/player/states/GroundedActionState.h"

class IdleActionState : public GroundedActionState {
private:
	std::string animation = "idle";
	CTimer closeCameraTimer;
	float closeCameraTime = 3.f;

public:
	IdleActionState(StateManager* stateManager);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
};