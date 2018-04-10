#pragma once

#include "comp_base.h"

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
    void debugInMenu();
    static void registerMsgs();
    void load(const json& j, TEntityParseContext& ctx);

    void onAllScenesCreated(const TMsgAllScenesCreated& msg);
    void update(float dt);

	void onActivate(const TMsgMechanismActivated& msg);
	void onDeactivate(const TMsgMechanismDeactivated& msg);
};
