#include "mcv_platform.h"
#include "comp_breakable.h"
#include "components/comp_transform.h"

DECL_OBJ_MANAGER("breakable", TCompBreakable);

void TCompBreakable::debugInMenu() {
}

void TCompBreakable::registerMsgs() {
	DECL_MSG(TCompBreakable, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompBreakable, TMsgColliderDestroyed, onColliderDestroyed);
}

void TCompBreakable::load(const json& j, TEntityParseContext& ctx) {
	
}

void TCompBreakable::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
}

void TCompBreakable::update(float dt) {
	
}



void TCompBreakable::onColliderDestroyed(const TMsgColliderDestroyed& msg) {
	CHandle(this).getOwner().destroy();
}
