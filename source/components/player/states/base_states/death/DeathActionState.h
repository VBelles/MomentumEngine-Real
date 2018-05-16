#pragma once

#include "components/player/states/IActionState.h"

class DeathActionState : public IActionState {
private:
	bool finish = false;
	std::string animation = "walk";

public:
	DeathActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;

	void respawn();
};
