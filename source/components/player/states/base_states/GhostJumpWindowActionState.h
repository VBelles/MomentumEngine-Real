#pragma once

#include "components/player/states/AirborneActionState.h"

class GhostJumpWindowActionState : public AirborneActionState {
	CTimer timer;
	float squatTime = frames2sec(6);
	float enteringVelocity = 0.f;
public:
	GhostJumpWindowActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override;
	void onJumpLongButton() override;
};