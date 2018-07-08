#pragma once

#include "geometry/transform.h"
#include "geometry/rigid_anim.h"

struct TCompRigidAnimsDirector : public TCompBase {
private:
	std::vector<std::string> animatedEntitiesNames;
	std::vector<CHandle> animatedEntities;
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
public:
	DECL_SIBLING_ACCESS();
	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();
	static void registerMsgs();
	void setIsMoving(bool isMoving);
	void setStopOnNextLoop(bool stop);
};
