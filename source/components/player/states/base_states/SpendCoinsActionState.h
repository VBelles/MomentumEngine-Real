#pragma once

#include "components/player/states/GroundedActionState.h"

class SpendCoinsActionState : public GroundedActionState {
private:
	CTimer spendTimer;

public:
	SpendCoinsActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onStrongAttackButton() override {}
	void onFastAttackButton() override {}
	//estos quiz� mejor sin override
	void onReleasePowerButton() override {}
	//void onDodgeButton() override {}
};