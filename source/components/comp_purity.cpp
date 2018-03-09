#include "mcv_platform.h"
#include "comp_purity.h"
#include "render/render_utils.h"
#include "player/comp_player_model.h"

DECL_OBJ_MANAGER("purity", TCompPurity);

void TCompPurity::debugInMenu() {
}

void TCompPurity::registerMsgs() {
	DECL_MSG(TCompPurity, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompPurity, TMsgPowerLvlChange, onPowerLvlChange);
}

void TCompPurity::load(const json& j, TEntityParseContext& ctx) {
}

void TCompPurity::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
    player = (CHandle)getEntityByName("The Player");

    collider = get<TCompCollider>();
    assert(collider);
}

void TCompPurity::onPowerLvlChange(const TMsgPowerLvlChange & msg) {
    if (msg.powerLvl == 1) {
        collider->enable();
    }
    else {
        collider->disable();
    }
}

void TCompPurity::update(float dt) {
}
