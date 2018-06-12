#pragma once

#include "components/player/states/GroundedActionState.h"

class JumpSquatLongActionState : public GroundedActionState {
private:
	CTimer timer;
	float squatTime = frames2sec(8);

	std::string animationSquat = "longJump_squat";
	std::string animationJump = "longJump_salto";

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