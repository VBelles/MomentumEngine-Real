#include "mcv_platform.h"
#include "comp_breakable.h"
#include "components/comp_transform.h"
#include "components/comp_collectable.h"
#include "components/comp_collider.h"
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
	prefabToSpawn = j.value("prefab", "");
	lootOffset = j.value("lootOffset", 1.0f);
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
	((CEntity*)(CHandle(this).getOwner()))->sendMsg(TMsgDestroy{});
	getCollider()->destroy();
	//send message to entity
}

void TCompBreakable::dropLoot() {
	TEntityParseContext ctx;
	VEC3 position = getTransform()->getPosition() + VEC3::Up * lootOffset;
	ctx.root_transform.setPosition(position);
	parseScene(prefabToSpawn, ctx);
}

void TCompBreakable::onColliderDestroyed(const TMsgColliderDestroyed& msg) {
	CHandle(this).getOwner().destroy();
	if (!prefabToSpawn.empty()) {
		dropLoot();
	}
}




