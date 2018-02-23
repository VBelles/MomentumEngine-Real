#pragma once

#include "components/player/comp_player_model.h"
#include "../../AirborneActionState.h"

class GhostJumpSquatLongActionState : public AirborneActionState {
	CTimer timer;
	int squatFrames = 2;
	float squatTime;
	float enteringVelocity = 0.f;
public:
	GhostJumpSquatLongActionState(TCompPlayerModel* player);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void SetMovementInput(VEC2 input) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;
	void OnLanding() override;
};