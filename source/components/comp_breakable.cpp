#include "mcv_platform.h"
#include "comp_breakable.h"
#include "components/comp_transform.h"

DECL_OBJ_MANAGER("breakable", TCompBreakable);

void TCompBreakable::debugInMenu() {
}

void TCompBreakable::registerMsgs() {
	DECL_MSG(TCompBreakable, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompBreakable, TMsgColliderDestroyed, onColliderDestroyed);
	DECL_MSG(TCompBreakable, TMsgAttackHit, onHit);
}

void TCompBreakable::load(const json& j, TEntityParseContext& ctx) {
	hp = j.value("hp", 1.f);
}

void TCompBreakable::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	colliderHandle = get<TCompCollider>();
}

void TCompBreakable::update(float dt) {

}

void TCompBreakable::onHit(const TMsgAttackHit & msg) {
	hp -= 1;
	if (hp <= 0) {
		onDie();
	}
}

void TCompBreakable::onDie() {
	getCollider()->destroy();
}

void TCompBreakable::dropLoot() {
	
}

void TCompBreakable::onColliderDestroyed(const TMsgColliderDestroyed& msg) {
	dropLoot();
	CHandle(this).getOwner().destroy();
}




