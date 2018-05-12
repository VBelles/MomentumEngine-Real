#pragma once

#include "components/player/states/AirborneActionState.h"

class WallJumpSquatActionState : public AirborneActionState {
protected:
	CTimer timer;
	float endingTime = frames2sec(4);

public:
	WallJumpSquatActionState(CHandle playerModelHandle) : AirborneActionState(playerModelHandle, "jump_inicio") {}
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}
};