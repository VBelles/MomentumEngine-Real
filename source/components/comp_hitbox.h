#pragma once

#include "comp_base.h"
#include "geometry/transform.h"
#include "entity/common_msgs.h"
#include "PxPhysicsAPI.h"
#include <set>

struct TMsgHitboxEnter {
	CHandle h_other_entity;
	DECL_MSG_ID();
};

class TCompHitbox : public TCompBase {
	DECL_SIBLING_ACCESS();

	VEC3 offset = {0,0,0};
	bool enabled = true;
	std::set<CHandle> handles;

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