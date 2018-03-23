#pragma once

#include "components/player/comp_player_model.h"


class GroundedActionState : public IActionState {
protected:
	float backwardsdMinAngle = 130.f;
	float backwardsMaxDotProduct;
public:
	GroundedActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;
	void OnStrongAttackButton() override;
	void OnFastAttackButton() override;
	void OnReleasePowerButton() override;

	virtual void OnLeavingGround();
};