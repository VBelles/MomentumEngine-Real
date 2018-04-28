#pragma once

#include "GrabActionState.h"

class GrabHighActionState : public GrabActionState {
public:
	GrabHighActionState(CHandle playerModelHandle)
		: GrabActionState(playerModelHandle) {}
	void onHitboxEnter(CHandle entity) override;
};