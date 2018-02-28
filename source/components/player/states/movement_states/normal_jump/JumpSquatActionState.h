#pragma once

#include "components/player/comp_player_model.h"
#include "../../GroundedActionState.h"

class JumpSquatActionState : public GroundedActionState {
	CTimer timer;
	int squatFrames = 5;
	float squatTime;

	bool isShortHop = false;

	float enteringVelocity = 0.f;
public:
	JumpSquatActionState(CHandle playerHandle);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void SetMovementInput(VEC2 input) override;
	void OnJumpHighButton() override;
	void OnJumpHighButtonReleased() override;

	virtual void OnLeavingGround() override;
};