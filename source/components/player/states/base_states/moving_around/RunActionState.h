#pragma once

#include "components/player/states/GroundedActionState.h"

class RunActionState : public GroundedActionState {
private:
	std::string animations[3] = { "run1", "run2", "run3" };
	std::string animation = "run1";
	int dustParticlesHandle = 0;

	bool hasSlash = false;
	CTimer movementTimer;

public:
	RunActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onSpendCoinsButton() override;
	void onLevelChange(int powerLevel);
};