#pragma once

#include "components/player/states/GroundedActionState.h"

class RunActionState : public GroundedActionState {
public:
	RunActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	
	void setPose() override;
};