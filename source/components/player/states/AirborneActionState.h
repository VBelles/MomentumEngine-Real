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
	float backwardsAirDriftFactor = 1.0f;
	PowerStats* enteringPowerStats;

public:
	AirborneActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;
	void OnStrongAttackButton() override;
	void OnFastAttackButton() override;

	virtual void OnLanding();

};