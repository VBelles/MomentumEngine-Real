#pragma once

#include "components/player/comp_player_model.h"
#include "../AirborneActionState.h"
#include "GrabActionState.h"


class GrabHighActionState : public GrabActionState {
public:
	GrabHighActionState(CHandle playerHandle, CHandle hitbox);
	void OnHitboxEnter(CHandle entity) override;
};