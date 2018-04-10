#pragma once

#include "components/comp_base.h"
#include "entity/common_msgs.h"

class TCompDestroyCondition: public TCompBase {
	DECL_SIBLING_ACCESS();

	bool toDestroy = false;

public:
    void debugInMenu();
    static void registerMsgs();
    void load(const json& j, TEntityParseContext& ctx);

    void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
    void update(float dt);

    void onDestroy(const TMsgDestroy& msg);
	void onColliderDestroyed(const TMsgColliderDestroyed& msg);
};
