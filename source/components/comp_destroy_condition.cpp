#include "mcv_platform.h"
#include "comp_destroy_condition.h"
#include "entity/entity_parser.h"
#include "components/comp_transform.h"

DECL_OBJ_MANAGER("destroyCondition", TCompDestroyCondition);

void TCompDestroyCondition::debugInMenu() {
}

void TCompDestroyCondition::registerMsgs() {
    DECL_MSG(TCompDestroyCondition, TMsgEntitiesGroupCreated, onGroupCreated);
    DECL_MSG(TCompDestroyCondition, TMsgDestroy, onDestroy);
}

void TCompDestroyCondition::load(const json& j, TEntityParseContext& ctx) {
}

void TCompDestroyCondition::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
    player = (CEntity*)getEntityByName("The Player");
}

void TCompDestroyCondition::update(float dt) {
}

void TCompDestroyCondition::onDestroy(const TMsgDestroy & msg) {
    dbg("Door: Destroying myself\n");
    CHandle(this).getOwner().destroy();

}
