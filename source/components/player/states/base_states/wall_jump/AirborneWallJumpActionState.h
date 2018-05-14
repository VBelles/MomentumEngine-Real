#pragma once

#include "components/player/states/AirborneActionState.h"

class AirborneWallJumpActionState : public AirborneActionState {
	
public:
	AirborneWallJumpActionState(CHandle playerModelHandle) : AirborneActionState(playerModelHandle, "jump_volando") {}
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
};