#pragma once

#include "GrabActionState.h"

class GrabHighActionState : public GrabActionState {
public:
	GrabHighActionState(StateManager* stageManager);

	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};