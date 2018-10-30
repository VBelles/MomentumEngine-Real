#pragma once

#include "components/player/states/GroundedActionState.h"

class SpendCoinsActionState : public GroundedActionState {
private:
	std::string animation = "spend_coins";
	std::string animationLoop = "spend_coins_loop";

	CTimer2 spendTimer;
	int coinsEachCycle = 1;
	float cycleTime = 7 / 60.f;
	float powerPerCoin = 2500.f;
	float timeToIncreasePower = cycleTime;
	
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

	float getPowerPerCoin() { return powerPerCoin; }
};
