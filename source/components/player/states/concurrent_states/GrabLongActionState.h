#pragma once

#include "GrabActionState.h"

class GrabLongActionState : public GrabActionState {
public:
	GrabLongActionState(StateManager * stageManager);

	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};
