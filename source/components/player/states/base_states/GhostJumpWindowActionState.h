#pragma once

#include "components/player/comp_player_model.h"
#include "../AirborneActionState.h"

class GhostJumpWindowActionState : public AirborneActionState {
	CTimer timer;
	int frameWindow = 6;
	float squatTime;
	float enteringVelocity = 0.f;
public:
	GhostJumpWindowActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override;
	void onJumpLongButton() override;
};