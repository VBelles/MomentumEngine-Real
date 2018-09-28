#pragma once

#include "entity/msgs.h"

struct TMsgEntitiesGroupCreated;
struct TMsgAllScenesCreated;
struct TMsgTriggerEnter;
struct TMsgColliderDestroyed;

class TCompPlayerModel;

class TCompPowerUp : public TCompBase {
	
private:
	std::string stateToUnlock;

	CHandle transformHandle;
	CHandle playerModelHandle;
	TCompTransform* getTransform();
	TCompPlayerModel* getPlayerModel();

public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);

	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onAllScenesCreated(const TMsgAllScenesCreated& msg);
	void onTriggerEnter(const TMsgTriggerEnter& msg);
	void onColliderDestroyed(const TMsgColliderDestroyed& msg);
};
