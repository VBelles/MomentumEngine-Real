#pragma once

#include "components/player/states/GroundedActionState.h"

class SpendCoinsActionState : public GroundedActionState {
private:
	std::string animation = "idle";

	CTimer spendTimer;
	int coinsEachCycle = 1;
	float cycleTime = 7 / 60.f;
	float powerPerCoin = 2500.f;
	float powerCycleTime = cycleTime;
	
public:
	SpendCoinsActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onStrongAttackButton() override {}
	void onFastAttackButton() override {}
	//estos quizá mejor sin override
	void onReleasePowerButton() override {}
	void onDodgeButton() override {}
	void onSpendCoinsButtonReleased() override;
};