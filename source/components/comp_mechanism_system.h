#pragma once

#include "comp_base.h"

struct TMsgAllScenesCreated;
struct TMsgMechanismActivated;
struct TMsgMechanismDeactivated;

class TCompMechanismSystem: public TCompBase {
	DECL_SIBLING_ACCESS();

private:
	CTimer deactivationTimer;
	float deactivationTime;
	bool isActivated = false;
	size_t numberOfMechanisms = 0;
	size_t numberOfMechanismsActivated = 0;

	std::vector<std::string> mechanismsNames;
	std::vector<CHandle> mechanisms;

public:
    static void registerMsgs();
    void debugInMenu();
    void load(const json& j, TEntityParseContext& ctx);
    void update(float dt);

    void onAllScenesCreated(const TMsgAllScenesCreated& msg);
	void onActivate(const TMsgMechanismActivated& msg);
	void onDeactivate(const TMsgMechanismDeactivated& msg);
};
