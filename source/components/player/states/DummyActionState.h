#pragma once

#include "components/player/states/IActionState.h"

class DummyActionState : public IActionState {
protected:

public:
	DummyActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onStrongAttackButton() override {}
	void onFastAttackButton() override {}
	void onReleasePowerButton() override {}
	void onDodgeButton() override {}
	void onMove(MoveState& moveState) override {}
	void onDamage(const TMsgAttackHit& msg) override {}
};
