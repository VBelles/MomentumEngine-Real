#pragma once

#include "components/player/states/AirborneActionState.h"

class SpringJumpActionState : public AirborneActionState {
private:
	std::string animation = "spring_jump_loop";
	std::string animationFalling = "free_fall";
	float driftSpeedThreshold = 10.f;

public:
	SpringJumpActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
};