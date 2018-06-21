#pragma once

#include "components/player/states/AirborneActionState.h"

class AirborneNormalActionState : public AirborneActionState {
private:
	std::string animation = "free_fall";

public:
	AirborneNormalActionState(StateManager* stateManager);

	void update(float delta) override;
	void onStateEnter(IActionState* lastState) override;
	void onStateExit(IActionState* nextState) override;
};