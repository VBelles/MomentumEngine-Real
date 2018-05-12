#pragma once

#include "components/player/states/GroundedActionState.h"

class JumpSquatSpringActionState : public GroundedActionState {
private:
	CTimer timer;
	float squatTime = frames2sec(12);

public:
	JumpSquatSpringActionState(CHandle playerModelHandle) : GroundedActionState(playerModelHandle, "jump_inicio") {}
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}
	virtual void onLeavingGround() override;
};