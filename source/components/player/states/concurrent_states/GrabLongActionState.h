#pragma once

#include "GrabActionState.h"

class GrabLongActionState : public GrabActionState {
public:
	GrabLongActionState(CHandle playerModelHandle)
		: GrabActionState(playerModelHandle) {
	}
	void onHitboxEnter(std::string hitbox, CHandle entity) override;
};