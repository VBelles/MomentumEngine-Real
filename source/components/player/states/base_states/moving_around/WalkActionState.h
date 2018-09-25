#pragma once

#include "components/player/states/GroundedActionState.h"

class WalkActionState : public GroundedActionState {
private:
	std::string animations[3] = { "walk", "walk", "walk_SS3" };
	std::string animation = "walk";
	int dustParticlesHandle = 0;

public:
	WalkActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onSpendCoinsButton() override;
	void onLevelChange(int powerLevel);
};