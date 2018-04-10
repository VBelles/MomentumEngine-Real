#pragma once

#include "components/player/comp_player_model.h"
#include "../GroundedActionState.h"


class WalkActionState : public GroundedActionState {
public:
	WalkActionState(CHandle playerModelHandle);
	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;

	void setPose() override;
};