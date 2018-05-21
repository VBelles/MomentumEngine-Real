#pragma once

#include "components/comp_base.h"

struct TMsgEntitiesGroupCreated;
struct TMsgTriggerEnter;

class TCompSpikes : public TCompBase {
private:
	float damage = 0.f;

public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void onTriggerEnter(const TMsgTriggerEnter & msg);	
};

