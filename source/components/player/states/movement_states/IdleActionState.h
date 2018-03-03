#pragma once

#include "components/player/comp_player_model.h"
#include "../GroundedActionState.h"


class IdleActionState : public GroundedActionState {
public:
	IdleActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void SetMovementInput(VEC2 input) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;
	virtual void OnLeavingGround();
};