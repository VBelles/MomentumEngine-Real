#pragma once

#include "components/player/states/IActionState.h"

class GroundedActionState : public IActionState {
protected:
	float backwardsdMinAngle = deg2rad(130.f);
	float backwardsMaxDotProduct;

	CTimer slideTimer;
	float slideWindowTime = 1.1f;
public:
	GroundedActionState(CHandle playerModelHandle, std::string animation = "") : IActionState(playerModelHandle, animation) {};
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override;
	void onJumpLongButton() override;
	void onStrongAttackButton() override;
	void onFastAttackButton() override;
	void onReleasePowerButton() override;
	void onMove(HitState& hitState) override;
	virtual void onLeavingGround();
	virtual void onDamage(float damage, bool isHard) override;
};