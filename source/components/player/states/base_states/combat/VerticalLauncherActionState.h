#pragma once

#include "LauncherActionState.h"

class VerticalLauncherActionState : public LauncherActionState {
public:
	VerticalLauncherActionState(StateManager* stateManager);
	void update(float delta) override;
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};
