#pragma once

#include "components/player/states/AirborneActionState.h"

class AirborneWallJumpActionState : public AirborneActionState {
private:
	std::string animationJump = "salto_pared";
	std::string animationLoop = "salto_pared_loop";

public:
	AirborneWallJumpActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
};