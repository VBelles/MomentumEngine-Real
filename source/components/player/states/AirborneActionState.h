#pragma once

#include "components/player/comp_player_model.h"

class AirborneActionState : public IActionState {
private:
	bool isTouching = false;
public:
	AirborneActionState(TCompPlayerModel* player);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
	void SetMovementInput(VEC2 input) override;
	void OnJumpHighButton() override;
	virtual void OnLanding();

};