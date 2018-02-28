#pragma once

#include "components/player/comp_player_model.h"
#include "../AirborneActionState.h"
#include "GrabActionState.h"


class GrabLongActionState : public GrabActionState {
public:
	GrabLongActionState(CHandle playerHandle, CHandle hitbox);
	void OnHitboxEnter(CHandle entity) override;
};