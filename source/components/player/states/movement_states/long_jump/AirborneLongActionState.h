#pragma once

#include "components/player/comp_player_model.h"
#include "../../AirborneActionState.h"

class AirborneLongActionState : public AirborneActionState {
private:
	VEC3 enterFront;
	float sidewaysdMinAngle = 45.f;
	float backwardsdMinAngle = 135.f;
	float sidewaysMaxDotProduct;
	float backwardsMaxDotProduct;
	float sidewaysAirDriftFactor = 0.5f;
	float backwardsAirDriftFactor = 0.8f;
public:
	AirborneLongActionState(TCompPlayerModel* player);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void SetMovementInput(VEC2 input) override;
	void OnJumpHighButton() override;
	virtual void OnLanding();

};