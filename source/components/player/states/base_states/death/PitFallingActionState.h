#pragma once

#include "components/player/states/IActionState.h"

class TCompScreenTransition;

class PitFallingActionState : public IActionState {
private:
	bool finish = false;
	CTimer2 timer;
	float respawnTime = 0.7f;
	float fallingDamage = 1;
	int frameCounter = 0;
	std::string animation = "free_fall";
	std::string animationIdle = "idle";

	bool screenTransitionStarted = false;
	TCompScreenTransition* getScreenTransition();

public:
	PitFallingActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void respawn();

	void copyCam();

};
