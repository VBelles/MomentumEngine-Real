#pragma once

#include "components/player/states/AirborneActionState.h"

class AirborneLongActionState : public AirborneActionState {
public:
	AirborneLongActionState(CHandle playerModelHandle) : AirborneActionState(playerModelHandle, "longJump_volando") {}
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
};