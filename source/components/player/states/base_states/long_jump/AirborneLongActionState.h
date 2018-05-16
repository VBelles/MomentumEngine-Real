#pragma once

#include "components/player/states/AirborneActionState.h"

class AirborneLongActionState : public AirborneActionState {
private:
	std::string animation = "longJump_volando";

public:
	AirborneLongActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
};