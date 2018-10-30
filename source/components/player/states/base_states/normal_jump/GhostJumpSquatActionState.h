#pragma once

#include "components/player/states/AirborneActionState.h"

class GhostJumpSquatActionState : public AirborneActionState {
private:
	CTimer2 timer;
	float squatTime = frames2sec(2);
	float enteringVelocity = 0.f;

	std::string animationJump = "normal_jump";
public:
	GhostJumpSquatActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}
};
