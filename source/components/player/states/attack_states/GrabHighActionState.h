#pragma once

#include "components/player/comp_player_model.h"
#include "../AirborneActionState.h"
#include "GrabActionState.h"


class GrabHighActionState : public GrabActionState {
public:
	GrabHighActionState(TCompPlayerModel* player, CHandle hitbox);
	void OnHitboxEnter(CHandle entity) override;
};