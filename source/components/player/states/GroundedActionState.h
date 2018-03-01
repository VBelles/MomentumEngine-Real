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
	void SetMovementInput(VEC2 input) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;
	void OnStrongAttackButton() override;
	void OnFastAttackButton() override;

	virtual void OnLeavingGround();
};