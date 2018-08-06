#pragma once

#include "components/comp_base.h"

struct TMsgEntitiesGroupCreated;
struct TMsgTriggerEnter;

class TCompTeleportPoint : public TCompBase {
private:

public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void update(float delta);

	void load(const json& j, TEntityParseContext& ctx);
	void onTriggerEnter(const TMsgTriggerEnter & msg);
	void onGroupCreated(const TMsgEntitiesGroupCreated & msg);
};

