#pragma once

#include "components/player/comp_player_model.h"
#include "../../AirborneActionState.h"

class AirborneLongActionState : public AirborneActionState {
public:
	AirborneLongActionState(TCompPlayerModel* player);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void SetMovementInput(VEC2 input) override;
	void OnJumpHighButton() override;
	void OnJumpLongButton() override;

	virtual void OnLanding() override;

};