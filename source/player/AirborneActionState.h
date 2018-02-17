#pragma once

#include "components/comp_player_model.h"


class AirborneActionState : public IActionState {
public:
	AirborneActionState(TCompPlayerModel* player);
	void update(float delta) override;
	void OnStateEnter(IActionState* lastState) override;
	void OnStateExit(IActionState* nextState) override;
};