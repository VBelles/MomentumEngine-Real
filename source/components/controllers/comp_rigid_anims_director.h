#pragma once

#include "geometry/transform.h"
#include "geometry/rigid_anim.h"

class TCompRigidAnimsDirector : public TCompBase {
private:

	bool isMoving = true;

	std::vector<std::string> animatedEntitiesNames;
	std::vector<CHandle> animatedEntities;
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
public:
	DECL_SIBLING_ACCESS();
	void load(const json& j, TEntityParseContext& ctx);
	void debugInMenu();
	static void registerMsgs();
	void setIsMoving(bool isMoving);
	bool getIsMoving() { return isMoving; }
	void setStopOnNextLoop(bool stop);
};
