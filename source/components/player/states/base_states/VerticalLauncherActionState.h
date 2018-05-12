#pragma once

#include "LauncherActionState.h"

class VerticalLauncherActionState : public LauncherActionState {
public:
	VerticalLauncherActionState(CHandle playerModelHandle) : LauncherActionState(playerModelHandle, "vertical_launcher"){
		//set here variables you want to override
		hitbox = "vertical_launcher";
	}
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};