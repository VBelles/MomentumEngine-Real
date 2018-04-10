#pragma once

#include "GrabActionState.h"

class GrabLongActionState : public GrabActionState {
public:
	GrabLongActionState(CHandle playerModelHandle, CHandle hitbox);
	void onHitboxEnter(CHandle entity) override;
};