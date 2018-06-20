#pragma once

#include "components/player/states/GroundedActionState.h"

class JumpSquatLongActionState : public GroundedActionState {
private:
	CTimer timer;
	float squatTime = frames2sec(8);

	std::string animation = "long_jump_squat";

public:
	JumpSquatLongActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}
	void onLeavingGround() override;
};