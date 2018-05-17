#pragma once

#include "components/comp_base.h"
#include "components/player/attack_info.h"

struct TMsgEntitiesGroupCreated;
struct TMsgTriggerEnter;

class TCompSpikes : public TCompBase {
private:
	float damage = 0.f;

public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void onGroupCreated(const TMsgEntitiesGroupCreated & msg);
	void onTriggerEnter(const TMsgTriggerEnter & msg);
	void update(float delta);
	
};

