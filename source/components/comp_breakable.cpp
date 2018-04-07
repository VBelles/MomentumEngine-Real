#include "mcv_platform.h"
#include "comp_breakable.h"
#include "components/comp_transform.h"
#include "components/comp_collectable.h"
#include "entity/entity_parser.h"

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
	transformHandle = get<TCompTransform>();
}

void TCompBreakable::update(float dt) {

}

void TCompBreakable::onHit(const TMsgAttackHit & msg) {
	if (msg.info.damage > 0.f) {
		hp -= 1;
		if (hp <= 0) {
			onDie();
		}
	}
}

void TCompBreakable::onDie() {
	getCollider()->destroy();
}

void TCompBreakable::dropLoot() {
	TEntityParseContext ctx;
	if (parseScene("data/prefabs/crisalida.prefab", ctx)) {
		CEntity* coinEntity = ctx.current_entity;
		TCompTransform* coinTransform = coinEntity->get<TCompTransform>();
		TCompCollectable* collectable = coinEntity->get<TCompCollectable>();
		coinTransform->setPosition(getTransform()->getPosition() + VEC3::Up * 1);
	}
}

void TCompBreakable::onColliderDestroyed(const TMsgColliderDestroyed& msg) {
	dropLoot();
	CHandle(this).getOwner().destroy();
}




