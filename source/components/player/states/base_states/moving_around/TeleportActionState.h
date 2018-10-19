#pragma once

#include "components/player/states/IActionState.h"

class TCompScreenTransition;

class TeleportActionState : public IActionState {
private:
	bool finish = false;
	CTimer timer;
	float respawnTime = 0.7f;
	float fallingDamage = 1;
	int frameCounter = 0;
	std::string animation = "cinematic_sleep_loop";

	bool screenTransitionStarted = false;
	TCompScreenTransition* getScreenTransition();

public:
	TeleportActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void respawn();

	void copyCam();

};
