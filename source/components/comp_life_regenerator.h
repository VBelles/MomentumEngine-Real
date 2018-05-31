#pragma once

#include "components/comp_base.h"

struct TMsgEntitiesGroupCreated;
struct TMsgTriggerEnter;

class TCompLifeRegenerator : public TCompBase {
private:
	float hpRestored = 1.f;
	CHandle player;

public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void load(const json& j, TEntityParseContext& ctx);
	void onTriggerEnter(const TMsgTriggerEnter & msg);	
};
