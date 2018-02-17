#pragma once

#include "mcv_platform.h"
#include "components/comp_player_model.h"

class TCompPlayerModel;

class IActionState {
protected:
	IActionState* lastState;
	IActionState* nextState;
	TCompPlayerModel* player;
public:
	IActionState(TCompPlayerModel* player) {
		this->player = player;
	}
	virtual void update(float delta) = 0;
	virtual void OnStateEnter(IActionState* lastState) { this->lastState = lastState; }
	virtual void OnStateExit(IActionState* nextState) { this->nextState = nextState; }
	virtual void OnJumpHighButton() { }

};
