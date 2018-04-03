#pragma once

#include "components/comp_base.h"
#include "geometry/transform.h"

class TCompPlayerController : public TCompBase {
	DECL_SIBLING_ACCESS();

private:
	float padDeadZone = 0.1f;
	void OnGroupCreated(const TMsgEntitiesGroupCreated& msg);

public:
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
};
