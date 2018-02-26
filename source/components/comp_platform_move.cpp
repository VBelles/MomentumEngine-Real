#include "mcv_platform.h"
#include "comp_platform_move.h"
//#include "entity/entity_parser.h"
#include "components/comp_transform.h"

DECL_OBJ_MANAGER("platform_move", TCompPlatformMove);

void TCompPlatformMove::debugInMenu() {
}

void TCompPlatformMove::registerMsgs() {
    DECL_MSG(TCompPlatformMove, TMsgEntitiesGroupCreated, onGroupCreated);
}

void TCompPlatformMove::load(const json& j, TEntityParseContext& ctx) {
    type = j.value("type", "NONE");
}

void TCompPlatformMove::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
}

void TCompPlatformMove::update(float dt) {
    TCompTransform* transform = get<TCompTransform>();
    if (!transform) return;


}
