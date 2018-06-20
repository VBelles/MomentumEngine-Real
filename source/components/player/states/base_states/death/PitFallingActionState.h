#pragma once

#include "components/player/states/IActionState.h"

class PitFallingActionState : public IActionState {
private:
	bool finish = false;
	float fallingDamage = 1;
	std::string animation = "free_fall";

public:
	PitFallingActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void respawn();

};