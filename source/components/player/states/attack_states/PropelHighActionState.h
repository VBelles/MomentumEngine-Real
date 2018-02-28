#pragma once

#include "components/player/comp_player_model.h"
#include "../AirborneActionState.h"


class PropelHighActionState : public AirborneActionState {
protected:
	CTimer timer;
	int endingFrames = 10;
	float endingTime;
	VEC3 movingVelocity;
	CHandle propelTarget;

public:
	PropelHighActionState(TCompPlayerModel* player);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void SetMovementInput(VEC2 input) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;

	void OnLanding() override;
};