#pragma once

#include "components/player/states/IActionState.h"

class DeathActionState : public IActionState {
private:
	CTimer timer;
	float deathTime = frames2sec(100);
	bool finish = false;
	std::string animation = "death";

public:
	DeathActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;

	void respawn();
};
