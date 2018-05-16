#pragma once

#include "LauncherActionState.h"

class HorizontalLauncherActionState : public LauncherActionState {

public:
	HorizontalLauncherActionState(StateManager* stateManager);

	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};