#pragma once

#include "components/player/states/AirborneActionState.h"

class HardKnockbackAirActionState : public AirborneActionState {
private:
	CTimer timer;
	float duration = 0.8f;

	std::string animation = "hard_knockback_air";
	std::string animationLoop = "hard_knockback_air_falling";

public:
	HardKnockbackAirActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;

	void onMove(MoveState & moveState) override;

	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onDodgeButton() override {}
	void onReleasePowerButton() override {}
};
