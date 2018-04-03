#pragma once

#include "comp_base.h"
#include "PxPhysicsAPI.h"
#include "entity/common_msgs.h"

using namespace physx;

struct TMsgTriggerEnter {
	CHandle h_other_entity;
	DECL_MSG_ID();
};

struct TMsgTriggerExit {
	CHandle h_other_entity;
	DECL_MSG_ID();
};

class TCompCollider : public TCompBase {
	DECL_SIBLING_ACCESS();

private:
	bool enabled = false;

public:
	struct TConfig {
		VEC3 halfExtent;
		VEC3 offset = { 0, 0, 0 };
		VEC4 plane;
		float step;
		float radius;
		float height;
		float slope;
		PxGeometryType::Enum shapeType;
		unsigned int group;
		unsigned int mask;
		bool is_dynamic;
		bool is_trigger;
		bool is_character_controller;
	};

	TConfig config;
	PxActor* actor;
	PxController* controller;
	void debugInMenu();
	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void onCreate(const TMsgEntityCreated& msg);
	void enable();
	void disable();
	bool isEnabled() { return enabled; }
	void setupFiltering(PxU32 filterGroup, PxU32 filterMask);

};
