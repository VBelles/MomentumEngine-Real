#pragma once

#include "comp_base.h"
#include "entity/msgs.h"

class CCollisionMesh;
struct TMsgEntityCreated;

using namespace physx;

struct TMsgTriggerEnter {
	CHandle h_other_entity;
	DECL_MSG_ID();
};

struct TMsgTriggerExit {
	CHandle h_other_entity;
	DECL_MSG_ID();
};

struct TMsgColliderDestroyed {
	DECL_MSG_ID();
};

struct ColliderConfig {
	std::string type;
	PxGeometryType::Enum shapeType;
	VEC4 plane;
	VEC3 halfExtent;
	VEC3 offset;
	float radius;
	float height;
	float step;
	float slope;
	unsigned int group;
	unsigned int mask;
	bool isTrigger;

    const CCollisionMesh* colMesh = nullptr;
};

class TCompCollider : public TCompBase {
	DECL_SIBLING_ACCESS();

private:
	bool created = false;
	void setCreated(bool value) { created = value; }
public:
	ColliderConfig config;
	PxActor* actor;
	PxController* controller;

	//Parent methods
	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);

	void onCreate(const TMsgEntityCreated& msg);
	void create();
	void destroy();
	void enableSimulation();
	void disableSimulation();
	void enableSceneQuery();
	void disableSceneQuery();
	bool isCreated() { return created; }
	void setupFiltering(PxU32 filterGroup, PxU32 filterMask);

};

