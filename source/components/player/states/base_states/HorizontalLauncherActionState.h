#pragma once

#include "LauncherActionState.h"

class HorizontalLauncherActionState : public LauncherActionState {
public:
	HorizontalLauncherActionState(CHandle playerModelHandle) : LauncherActionState(playerModelHandle, "vertical_launcher") {
		//set here variables you want to override
		hitbox = "horizontal_launcher";
	}
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};