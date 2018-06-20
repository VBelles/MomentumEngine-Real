#pragma once

#include "components/player/states/AirborneActionState.h"

class GhostJumpWindowActionState : public AirborneActionState {
private:
	CTimer timer;
	float squatTime = frames2sec(6);
	float enteringVelocity = 0.f;

	std::string animation = "free_fall";

public:
	GhostJumpWindowActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override;
	void onJumpLongButton() override;
};