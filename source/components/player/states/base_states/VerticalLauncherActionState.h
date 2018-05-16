#pragma once

#include "LauncherActionState.h"

class VerticalLauncherActionState : public LauncherActionState {
public:
	VerticalLauncherActionState(StateManager* stateManager);

	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};