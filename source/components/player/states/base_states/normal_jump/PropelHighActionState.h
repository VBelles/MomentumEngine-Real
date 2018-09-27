#pragma once

#include "components/player/states/AirborneActionState.h"

class PropelHighActionState : public AirborneActionState {
protected:
	CTimer timer;
	float endingTime = frames2sec(10);
	VEC3 movingVelocity;
	CHandle propelTarget;

	float invulnerableTime = 0.5f;

	float propelDuration = 0.5f;

	std::string animation = "propel_high";

public:
	PropelHighActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onDodgeButton() override {}
	void onReleasePowerButton() override {}
	void onLanding() override {}
};
