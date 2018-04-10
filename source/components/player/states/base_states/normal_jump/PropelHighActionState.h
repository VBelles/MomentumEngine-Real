#pragma once

#include "components/player/comp_player_model.h"
#include "components/player/states/AirborneActionState.h"


class PropelHighActionState : public AirborneActionState {
protected:
	CTimer timer;
	int endingFrames = 10;
	float endingTime;
	VEC3 movingVelocity;
	CHandle propelTarget;

public:
	PropelHighActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}

	void onLanding() override {}
	void setPose() override;
};