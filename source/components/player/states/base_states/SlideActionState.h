#pragma once

#include "components/player/states/AirborneActionState.h"

class SlideActionState : public AirborneActionState {
public:
	SlideActionState(CHandle playerModelHandle) :AirborneActionState(playerModelHandle, "jump_volando") {}
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onMove(HitState& hitState) override;
};