#pragma once

#include "components/player/states/AirborneActionState.h"

class SlideActionState : public AirborneActionState {
private:
	VEC3 hitNormal;
	float maxSlidingVelocity = 10.f;

	bool isTryingToLand = false;
	float landingWindowTime = 0.1f;
	CTimer landingWindowTimer;
public:
	SlideActionState(CHandle playerModelHandle) :AirborneActionState(playerModelHandle, "jump_volando") {}
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;

	void onJumpHighButton() override {}
	void onJumpLongButton() override {}
	void onFastAttackButton() override {}
	void onStrongAttackButton() override {}
	void onReleasePowerButton() override {}
	void onMove(HitState& hitState) override;
	void onShapeHit(const PxControllerShapeHit & hit) override {}
};