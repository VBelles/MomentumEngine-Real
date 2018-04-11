#pragma once

#include "comp_base.h"

struct TMsgEntityCreated;
struct TMsgEntitiesGroupCreated;
struct TMsgTriggerEnter;

class TCompHitbox : public TCompBase {
	DECL_SIBLING_ACCESS();

private:
	VEC3 offset = {0,0,0};
	std::set<CHandle> handles;
	bool enabled = true;
public:
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	void onCreate(const TMsgEntityCreated& msg);
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onTriggerEnter(const TMsgTriggerEnter& msg);
	void disable();
	void enable();

	VEC3 getOffset();
	void setOffset(VEC3 newOffset);
};