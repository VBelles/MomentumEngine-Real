#pragma once

#include "LauncherActionState.h"

class HorizontalLauncherActionState : public LauncherActionState {

public:
	HorizontalLauncherActionState(StateManager* stateManager);
	void update(float delta) override;
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};
