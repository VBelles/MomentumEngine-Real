#pragma once

#include "components/player/states/GroundedActionState.h"

class RunActionState : public GroundedActionState {
private:
	std::string animations[3] = { "runSS1", "runSS2", "runSS3" };
	std::string animation = "runSS1";
	int dustParticlesHandle = 0;

public:
	RunActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onSpendCoinsButton() override;

	void setAnimation(int powerLevel);
};