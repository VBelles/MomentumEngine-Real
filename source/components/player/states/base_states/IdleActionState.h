#pragma once

#include "components/player/states/GroundedActionState.h"

class IdleActionState : public GroundedActionState {
public:
	IdleActionState(CHandle playerModelHandle) : GroundedActionState(playerModelHandle, "idle") {}
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
};