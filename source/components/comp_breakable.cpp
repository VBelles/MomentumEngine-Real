#include "mcv_platform.h"
#include "comp_breakable.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"
#include "components/comp_collectable.h"
#include "components/controllers/comp_rigid_anims_director.h"
#include "components/player/attack_info.h"


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

	if (j.count("broken_parts")) {
		auto& entities = j["broken_parts"];
		assert(entities.is_array());
		for (std::string entity : entities) {
			brokenPartsNames.push_back(entity);
		}
	}
}

void TCompBreakable::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	colliderHandle = get<TCompCollider>();
	assert(colliderHandle.isValid());
	transformHandle = get<TCompTransform>();
	assert(transformHandle.isValid());

	for (std::string name : brokenPartsNames) {
		CHandle entity = getEntityByName(name);
		if (entity.isValid()) {
			brokenParts.push_back(entity);
		}
	}
}

void TCompBreakable::onHit(const TMsgAttackHit & msg) {
	CEntity* entity = msg.attacker;
	if (entity->getName() == PLAYER_NAME && msg.info.damage > 0.f) {
		hp -= 1;
		if (hp <= 0) {
			onDie();
		}
	}
}

void TCompBreakable::onDie() {
	//activate broken parts
	for (CEntity* brokenPart : brokenParts) {
		TCompRender* render = brokenPart->get<TCompRender>();
		render->enable();
	}
	TCompRigidAnimsDirector* director = get<TCompRigidAnimsDirector>();
	if (director) {
		director->setIsMoving(true);
	}
	((CEntity*)(CHandle(this).getOwner()))->sendMsg(TMsgDestroy{});
	getCollider()->destroy();
	EngineSound.emitEvent(SOUND_BREAKABLE, getTransform());
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


