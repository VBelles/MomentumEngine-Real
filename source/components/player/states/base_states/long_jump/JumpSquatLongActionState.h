#pragma once

#include "components/player/states/GroundedActionState.h"

class JumpSquatLongActionState : public GroundedActionState {
private:
	CTimer timer;
	int squatFrames = 8;
	float squatTime;
public:
	JumpSquatLongActionState(CHandle playerModelHandle) : GroundedActionState(playerModelHandle, "longJump_inicio") {}
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}
	virtual void onLeavingGround();
};