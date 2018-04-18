#include "mcv_platform.h"
#include "scripting_entities.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "entity/entity_parser.h"
#include <SLB/SLB.hpp>

ScriptingEntities* ScriptingEntities::instance = nullptr;

ScriptingEntities::ScriptingEntities() {
	CEntity* playerEntity = getEntityByName(PLAYER_NAME);
	assert(playerEntity);
	playerTransformHandle = playerEntity->get<TCompTransform>();
}

ScriptingEntities::~ScriptingEntities() {
}


void ScriptingEntities::bind(SLB::Manager* manager) {
	assert(instance);
	manager->set("spawnGolemAt", SLB::FuncCall::create(ScriptingEntities::spawnGolemAt));
	manager->set("spawnGolem", SLB::FuncCall::create(ScriptingEntities::spawnGolem));
}

void ScriptingEntities::spawnGolemAt(float x, float y, float z) {
	TEntityParseContext ctx;
	CTransform transform;
	transform.setPosition(VEC3(x, y, z));
	ctx.root_transform = transform;
	parseScene("data/prefabs/enemy_melee.prefab", ctx);
}

void ScriptingEntities::spawnGolem() {
	VEC3 spawnPosition = get()->getPlayerTransform()->getPosition() + (get()->getPlayerTransform()->getFront() * 2);
	spawnGolemAt(spawnPosition.x, spawnPosition.y, spawnPosition.z);
}

TCompTransform * ScriptingEntities::getPlayerTransform() {
	return playerTransformHandle;
}




