#pragma once

#include "components/player/states/AirborneActionState.h"

class SlideActionState : public AirborneActionState {
private:
	VEC3 hitNormal;
	float maxVerticalSlidingVelocity = 10.f;
	float sidewaysSlidingVelocity = 2.f;

	std::string animation = "jump_volando";

public:
	SlideActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}
	void onMove(MoveState& moveState) override;
};