#pragma once

#include "components/player/states/GroundedActionState.h"

class JumpSquatSpringActionState : public GroundedActionState {
private:
	CTimer timer;
	float squatTime = frames2sec(12);

	std::string animation = "spring_jump_squat";

public:
	JumpSquatSpringActionState(StateManager* stateManager);
	
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
