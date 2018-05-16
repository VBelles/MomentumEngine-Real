#pragma once

#include "components/player/states/IActionState.h"

class FreeActionState : public IActionState {
private:

public:
	FreeActionState(StateManager* stateManager);

	void update(float delta) override {}
};