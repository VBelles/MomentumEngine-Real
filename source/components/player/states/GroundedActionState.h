#pragma once

#include "components/player/comp_player_model.h"


class GroundedActionState : public IActionState {
public:
	GroundedActionState(TCompPlayerModel* player);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void SetMovementInput(VEC2 input) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;

	virtual void OnLeavingGround();
};