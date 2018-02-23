#pragma once

#include "components/player/comp_player_model.h"
#include "../../GroundedActionState.h"

class JumpSquatActionState : public GroundedActionState {
	CTimer timer;
	int squatFrames = 4;
	float squatTime;
	float enteringVelocity = 0.f;
public:
	JumpSquatActionState(TCompPlayerModel* player);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void SetMovementInput(VEC2 input) override;
	void OnJumpHighButton() override;
	virtual void OnLeavingGround();
};