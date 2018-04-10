#include "mcv_platform.h"
#include "comp_breakable.h"
#include "components/comp_transform.h"
#include "components/comp_collectable.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"

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
	assert(colliderHandle.isValid());
	transformHandle = get<TCompTransform>();
	assert(transformHandle.isValid());
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
	if (parseScene("data/prefabs/coin.prefab", ctx)) {
		CEntity* coinEntity = ctx.current_entity;
		TCompTransform* coinTransform = coinEntity->get<TCompTransform>();
		TCompCollectable* collectable = coinEntity->get<TCompCollectable>();
		TCompCollider* collider = coinEntity->get<TCompCollider>();
		VEC3 position = getTransform()->getPosition() + VEC3::Up * 1;
		coinTransform->setPosition(position);
		PxRigidActor* actor = (PxRigidActor*) collider->actor;
		actor->setGlobalPose(PxTransform(toPhysx(position)));
	}
}

void TCompBreakable::onColliderDestroyed(const TMsgColliderDestroyed& msg) {
	CHandle(this).getOwner().destroy();
	dropLoot();
}




