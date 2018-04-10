#pragma once

#include "components/player/states/AirborneActionState.h"

class AirborneWallJumpActionState : public AirborneActionState {

private:
	float backwardsAirDriftFactorLong = 0.4f;
	float sidewaysAirDriftFactorLong = 0.1f;
public:
	AirborneWallJumpActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;

	void setPose() override;


};