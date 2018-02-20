#pragma once

#include "components/comp_player_model.h"
#include "AirborneActionState.h"

class GhostJumpSquatActionState : public AirborneActionState {
	CTimer timer;
	int squatFrames = 2;
	float squatTime;
	float enteringVelocity = 0.f;
public:
	GhostJumpSquatActionState(TCompPlayerModel* player);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void SetMovementInput(VEC2 input, float delta) override;
	void OnJumpHighButton() override;
	void OnLanding() override;
};