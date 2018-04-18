#include "mcv_platform.h"
#include "scripting_entities.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "components/comp_name.h"
#include "entity/entity_parser.h"
#include <SLB/SLB.hpp>

ScriptingEntities* ScriptingEntities::instance = nullptr;
int ScriptingEntities::spawnedGolemCount = 0;

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

std::string ScriptingEntities::spawnEntityAt(std::string prefabFilename, float x, float y, float z) {
	TEntityParseContext ctx;
	CTransform transform;
	transform.setPosition(VEC3(x, y, z));
	ctx.root_transform = transform;
	parseScene(prefabFilename, ctx);
	CEntity* entity = ctx.current_entity;
	TCompName* nameComp = entity->get<TCompName>();
	assert(nameComp);
	std::string name = "S" + std::string(nameComp->getName());
	nameComp->setName(name.c_str());
	return name;
}

std::string ScriptingEntities::spawnGolemAt(float x, float y, float z) {
	TEntityParseContext ctx;
	CTransform transform;
	transform.setPosition(VEC3(x, y, z));
	ctx.root_transform = transform;
	parseScene("data/prefabs/enemy_melee.prefab", ctx);
	CEntity* entity = ctx.current_entity;
	TCompName* nameComp = entity->get<TCompName>();
	assert(nameComp);
	std::string name = "SGolem" + std::to_string(spawnedGolemCount++);
	nameComp->setName(name.c_str());
	return name;
}

std::string ScriptingEntities::spawnGolem() {
	VEC3 spawnPosition = get()->getPlayerTransform()->getPosition() + (get()->getPlayerTransform()->getFront() * 2);
	return spawnGolemAt(spawnPosition.x, spawnPosition.y, spawnPosition.z);
}

TCompTransform * ScriptingEntities::getPlayerTransform() {
	return playerTransformHandle;
}




