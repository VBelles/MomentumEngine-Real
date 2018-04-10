#pragma once

#include "components/player/states/GroundedActionState.h"

class IdleActionState : public GroundedActionState {
public:
	IdleActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;

	void setPose() override;
};