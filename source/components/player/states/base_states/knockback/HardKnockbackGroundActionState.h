#pragma once

#include "components/player/states/GroundedActionState.h"

class HardKnockbackGroundActionState : public GroundedActionState {
private:
	CTimer timer;
	float duration = 1.5f;

public:
	HardKnockbackGroundActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;

	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}
};