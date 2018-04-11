#pragma once

#include "components/comp_base.h"

struct TMsgEntitiesGroupCreated;
class TCompCollider;
class TCompTransform;

class TCompPlatformRotative : public TCompBase {
	DECL_SIBLING_ACCESS();

private:
	CHandle colliderHandle;
	CHandle transformHandle;

	VEC3 rotationSpeed;

	//Messages
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);

	TCompCollider* getCollider();
	TCompTransform* getTransform();
	PxRigidDynamic* getRigidDynamic();

public:
	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
};
