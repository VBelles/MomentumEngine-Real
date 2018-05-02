#pragma once

#include "components/player/states/GroundedActionState.h"

class WalkActionState : public GroundedActionState {
public:
	WalkActionState(CHandle playerModelHandle) : GroundedActionState(playerModelHandle, "run") {}
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
};