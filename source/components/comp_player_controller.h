#pragma once

#include "comp_base.h"
#include "geometry/transform.h"
#include "comp_player_model.h"

class TCompPlayerController : public TCompBase {
	float padDeadZone = 0.1f;
	TCompPlayerModel* playerModel = get<TCompPlayerModel>();

	void OnGroupCreated(const TMsgEntitiesGroupCreated& msg);

	DECL_SIBLING_ACCESS();

public:
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
};