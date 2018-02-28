#pragma once

#include "components/player/comp_player_model.h"

class AirborneActionState : public IActionState {
protected:
	VEC3 enterFront;
	float sidewaysdMinAngle = 45.f;
	float backwardsdMinAngle = 135.f;
	float sidewaysMaxDotProduct;
	float backwardsMaxDotProduct;
	float sidewaysAirDriftFactor = 0.5f;
	float backwardsAirDriftFactor = 0.8f;

public:
	AirborneActionState(CHandle playerHandle);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void SetMovementInput(VEC2 input) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;
	void OnStrongAttackButton() override;
	void OnFastAttackButton() override;

	virtual void OnLanding();

};