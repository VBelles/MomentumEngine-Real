#pragma once

#include "components/player/states/AirborneActionState.h"

class GhostJumpSquatLongActionState : public AirborneActionState {
private:
	CTimer timer;
	float squatTime = frames2sec(2);
	float enteringVelocity = 0.f;
public:
	GhostJumpSquatLongActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}
};