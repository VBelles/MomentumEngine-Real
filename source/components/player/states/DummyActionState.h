#pragma once

#include "components/player/states/IActionState.h"

class DummyActionState : public IActionState {
protected:
	std::string animation = "get_chrysalis";
	std::string exitAnimation = "idle_SS3";
	bool isLoop = false;
	float duration = -1;//con 0 o menos loopea hasta nuevo aviso

	CTimer2 timer;

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

	void setDummyState(std::string animation, bool isLoop, float duration, std::string exitAnimation);
};
