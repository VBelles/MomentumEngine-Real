#pragma once

#include "components/comp_base.h"

struct TMsgEntityCreated;
struct TMsgEntitiesGroupCreated;
struct TMsgTriggerEnter;

class TCompHitboxes : public TCompBase {
private:
public:
	struct HitboxConfig {
		VEC3 offset;
		PxGeometryType::Enum shapeType;
		PxRigidActor* actor;
		VEC3 halfExtent;
		float radius;
		float height;
		unsigned int group;
		unsigned int mask;
	};
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);

	//Message callbacks
	void onCreate(const TMsgEntityCreated& msg);
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onTriggerEnter(const TMsgTriggerEnter& msg);
};