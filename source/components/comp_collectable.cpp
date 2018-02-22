#include "mcv_platform.h"
#include "comp_collectable.h"

DECL_OBJ_MANAGER("collectable", TCompCollectable);

void TCompCollectable::debugInMenu() {
}

void TCompCollectable::load(const json& j, TEntityParseContext& ctx) {

}

void TCompCollectable::OnCollect(const TMsgTriggerEnter & msg)
{
	// Check if collector is the player.
	CHandle(this).getOwner().destroy();
}

void TCompCollectable::registerMsgs() {
    DECL_MSG(TCompCollectable, TMsgEntityCreated, onCreate);
}

void TCompCollectable::onCreate(const TMsgEntityCreated& msg) {
}
