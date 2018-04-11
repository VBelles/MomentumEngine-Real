#pragma once

#include "GrabActionState.h"

class GrabHighActionState : public GrabActionState {
public:
	GrabHighActionState(CHandle playerModelHandle, CHandle hitbox);
	void onHitboxEnter(CHandle entity) override;
};