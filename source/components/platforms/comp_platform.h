#pragma once

#include "components/comp_base.h"


struct TMsgEntitiesGroupCreated;
struct TMsgTriggerEnter;
struct TMsgTriggerExit;

/* Se pone en el trigger de una plataforma y mueve al player con
   ella si lo detecta encima. */
class TCompPlatform: public TCompBase {
	DECL_SIBLING_ACCESS();

    CHandle player;
    bool    isPlayerInTrigger = false;

public:
    void debugInMenu();
    static void registerMsgs();
    void load(const json& j, TEntityParseContext& ctx);

    void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onTriggerEnter(const TMsgTriggerEnter& msg);
	void onTriggerExit(const TMsgTriggerExit& msg);

    void update(float dt);
};
