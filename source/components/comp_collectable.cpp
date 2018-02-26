#include "mcv_platform.h"
#include "comp_collectable.h"
//#include "entity/entity_parser.h"
#include "components/comp_transform.h"

DECL_OBJ_MANAGER("collectable", TCompCollectable);

void TCompCollectable::debugInMenu() {
}

void TCompCollectable::registerMsgs() {
    DECL_MSG(TCompCollectable, TMsgEntitiesGroupCreated, onGroupCreated);
    //DECL_MSG(TCompCollectable, TMsgTriggerEnter, onCollect);
}

void TCompCollectable::load(const json& j, TEntityParseContext& ctx) {
    type = j.value("type", "chrysalis");
}

void TCompCollectable::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
}

void TCompCollectable::update(float dt) {
    TCompTransform* transform = get<TCompTransform>();
    if (!transform) return;

    player = (CEntity*)getEntityByName("The Player");
    TCompTransform* playerTransform = player->get<TCompTransform>();
    assert(playerTransform);

    float distanceToPlayer = VEC3::Distance(transform->getPosition(),
                                            playerTransform->getPosition());
    if (distanceToPlayer < collision_radius) {
        CHandle(this).getOwner().destroy();
        TMsgCollect msg{ type };
        player->sendMsg(msg);
    }
}

//void TCompCollectable::onCollect(const TMsgTriggerEnter & msg) {
//    h_collector = msg.h_other_entity;
//    CEntity *collector = h_collector.getOwner();
//    dbg("Trigger enter by %s\n", collector->getName());
//    if (collector->getName() == "The Player") {
//        // Only the player can collect.
//	    CHandle(this).getOwner().destroy();
//    }
//}
