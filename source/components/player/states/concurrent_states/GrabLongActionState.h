#pragma once

#include "GrabActionState.h"

class GrabLongActionState : public GrabActionState {
public:
	GrabLongActionState(StateManager * stageManager);

	void onStateEnter(IActionState* lastState) override;

	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};
