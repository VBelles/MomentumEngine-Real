#pragma once

#include "components/player/states/GroundedActionState.h"

class SoftKnockbackGroundActionState : public GroundedActionState {
private:
	CTimer timer;
	float duration = 0.8f;

	std::string animation = "ataquecaida";

public:
	SoftKnockbackGroundActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;

	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}
};