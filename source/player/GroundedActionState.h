#pragma once

#include "components/comp_player_model.h"


class GroundedActionState : public IActionState {
public:
	GroundedActionState(TCompPlayerModel* player);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void OnJumpHighButton() override;
	virtual void OnLeavingGround();
};