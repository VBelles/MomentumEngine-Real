#pragma once

#include "components/player/states/GroundedActionState.h"

class JumpSquatActionState : public GroundedActionState {
private:
	CTimer timer;
	float squatTime = frames2sec(5);

	bool isShortHop = false;

	float enteringVelocity = 0.f;

public:
	JumpSquatActionState(CHandle playerModelHandle) : GroundedActionState(playerModelHandle, "jump_inicio") {}
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}
	void onJumpHighButtonReleased() override;
	virtual void onLeavingGround() override;
};