#pragma once

#include "components/player/states/GroundedActionState.h"

class IdleActionState : public GroundedActionState {
private:
	std::string animations[3] = { "idle", "idle", "idle_SS3" };
	std::string animation = "idle";
	CTimer closeCameraTimer;
	float closeCameraTime = 3.f;

public:
	IdleActionState(StateManager* stateManager);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onSpendCoinsButton() override;
	void onLevelChange(int powerLevel);
};
