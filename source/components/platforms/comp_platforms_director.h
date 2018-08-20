#pragma once

#include "geometry/transform.h"

class TCompPlatformsDirector : public TCompBase {
private:
	std::vector<std::vector<std::string>> platformEntitiesNames;
	std::vector<float> waitTimes;
	std::vector<std::vector<CHandle>> platformHandles;

	CTimer timer;
	int currentSlot = 0;
	float timeBetweenSlots = 2.f;
	float nextSlotTime = 0;

	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);

public:
	DECL_SIBLING_ACCESS();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	static void registerMsgs();
	void update(float delta);
	void setIsMoving(bool isMoving);
	void setStopOnNextLoop(bool stop);
};
