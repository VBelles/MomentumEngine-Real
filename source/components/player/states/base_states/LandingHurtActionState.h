#pragma once

#include "components/player/states/base_states/LandingActionState.h"

class LandingHurtActionState : public LandingActionState {
protected:
	std::string animation = "landing_hard";

public:
	LandingHurtActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	
	void setRemainingTime(float time);

	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onDodgeButton() override {}
	void onReleasePowerButton() override {}
};
