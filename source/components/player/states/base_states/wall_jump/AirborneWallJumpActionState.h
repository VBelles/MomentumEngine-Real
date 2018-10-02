#pragma once

#include "components/player/states/AirborneActionState.h"

class AirborneWallJumpActionState : public AirborneActionState {
private:
	std::string animation = "wall_jump_loop";

public:
	AirborneWallJumpActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
};
