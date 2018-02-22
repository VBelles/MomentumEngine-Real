#pragma once

#include "components/player/comp_player_model.h"
#include "AirborneActionState.h"

class AirborneNormalActionState : public AirborneActionState {
private:
	bool isTouching = false;
public:
	AirborneNormalActionState(TCompPlayerModel* player);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void SetMovementInput(VEC2 input) override;
	void OnJumpHighButton() override;
	virtual void OnLanding();

};