#pragma once

#include "components/player/comp_player_model.h"


class GroundedActionState : public IActionState {
protected:
	float backwardsdMinAngle = deg2rad(130.f);
	float backwardsMaxDotProduct;
public:
	GroundedActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
	void onJumpHighButton() override;
	void onJumpLongButton() override;
	void onStrongAttackButton() override;
	void onFastAttackButton() override;
	void onReleasePowerButton() override;

	virtual void onLeavingGround();
};