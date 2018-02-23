#include "mcv_platform.h"
#include "comp_collectable.h"
#include "entity/entity_parser.h"

DECL_OBJ_MANAGER("collectable", TCompCollectable);

void TCompCollectable::debugInMenu() {
}

void TCompCollectable::registerMsgs() {
    DECL_MSG(TCompCollectable, TMsgEntityCreated, onCreate);
    DECL_MSG(TCompCollectable, TMsgEntitiesGroupCreated, onGroupCreated);
    DECL_MSG(TCompCollectable, TMsgTriggerEnter, onCollect);
}

void TCompCollectable::load(const json& j, TEntityParseContext& ctx) {   

}

void TCompCollectable::onCreate(const TMsgEntityCreated& msg) {
}

void TCompCollectable::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
}

void TCompCollectable::onCollect(const TMsgTriggerEnter & msg) {
    h_collector = msg.h_other_entity;
    CEntity *collector = h_collector.getOwner();
    dbg("Trigger enter by %s\n", collector->getName());
    if (collector->getName() == "The Player") {
        // Only the player can collect.
	    CHandle(this).getOwner().destroy();
    }

}
