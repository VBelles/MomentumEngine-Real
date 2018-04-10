#pragma once

#include "components/player/comp_player_model.h"
#include "components/player/states/AirborneActionState.h"

class AirborneLongActionState : public AirborneActionState {
public:
	AirborneLongActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;

	void setPose() override;
};