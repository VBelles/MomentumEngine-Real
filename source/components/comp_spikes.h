#pragma once

#include "components/comp_base.h"

struct TMsgEntitiesGroupCreated;
struct TMsgTriggerEnter;

class TCompSpikes : public TCompBase {
	DECL_SIBLING_ACCESS();
private:
	float damage = 0.f;

public:
	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void onGroupCreated(const TMsgEntitiesGroupCreated & msg);
	void onTriggerEnter(const TMsgTriggerEnter & msg);
	void update(float delta);
	
};

