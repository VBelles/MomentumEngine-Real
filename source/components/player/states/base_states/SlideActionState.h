#pragma once

#include "components/player/states/AirborneActionState.h"

class SlideActionState : public AirborneActionState {
private:
	VEC3 hitNormal;
	float maxVerticalSlidingVelocity = 2.f;
	float sidewaysSlidingVelocity = 2.f;

	std::string animation = "free_fall";

	constexpr static float STUCK_TIME = 2.f;
	constexpr static float STUCK_THRESHOLD = 0.5f;
	bool canJump = false;
	float stuckY = 0.f;
	CTimer stuckTimer;

	bool isJumping = false;
	bool isLongJumping = false;


public:
	SlideActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override;
	void onJumpLongButton() override;
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}
	void onMove(MoveState& moveState) override;
};