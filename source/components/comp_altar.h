#pragma once

#include "comp_base.h"

struct TMsgAllScenesCreated;
struct TMsgDestroy;

class TCompAltar: public TCompBase {
	DECL_SIBLING_ACCESS();

private:
	CTimer deactivationTimer;
	float deactivationTime;
	bool isActivated = false;
	size_t numberOfPositions = 0;
	size_t numberOfMechanismsActivated = 0;
	std::string prefabToSpawn;

	std::vector<VEC3> spawnPositions;

public:
    static void registerMsgs();
    void debugInMenu();
    void load(const json& j, TEntityParseContext& ctx);

    void onAllScenesCreated(const TMsgAllScenesCreated& msg);
	void onDestroy(const TMsgDestroy& msg);
};
