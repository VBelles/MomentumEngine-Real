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
	DECL_MSG(TCompDestroyCondition, TMsgColliderDestroyed, OnColliderDestroyed);
}

void TCompDestroyCondition::load(const json& j, TEntityParseContext& ctx) {
}

void TCompDestroyCondition::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
}

void TCompDestroyCondition::update(float dt) {
}

void TCompDestroyCondition::onDestroy(const TMsgDestroy & msg) {
    // Door destroys itself.
    TCompCollider *collider = get<TCompCollider>();
	collider->disable();
}

void  TCompDestroyCondition::OnColliderDestroyed(const TMsgColliderDestroyed& msg) {
	CHandle(this).getOwner().destroy();
}