#pragma once

#include "components/player/comp_player_model.h"
#include "components/player/states/AirborneActionState.h"

class GhostJumpSquatLongActionState : public AirborneActionState {
private:
	CTimer timer;
	int squatFrames = 2;
	float squatTime;
	float enteringVelocity = 0.f;
public:
	GhostJumpSquatLongActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}
};