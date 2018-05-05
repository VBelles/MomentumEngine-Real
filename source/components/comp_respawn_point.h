#pragma once

#include "comp_base.h"
#include "entity/msgs.h"

struct TMsgTriggerEnter;

struct TMsgRespawnChanged {
	VEC3 respawnPosition;
	DECL_MSG_ID();
};

class TCompRespawnPoint : public TCompBase {

private:	
	VEC3 respawnPosition;

public:
	DECL_SIBLING_ACCESS();
    static void registerMsgs();
    void debugInMenu();
    void load(const json& j, TEntityParseContext& ctx);

	void onTriggerEnter(const TMsgTriggerEnter & msg);


};
