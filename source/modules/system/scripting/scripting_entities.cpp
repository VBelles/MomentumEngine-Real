#include "mcv_platform.h"
#include "scripting_entities.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "components/comp_name.h"
#include "entity/entity_parser.h"
#include <SLB/SLB.hpp>

ScriptingEntities* ScriptingEntities::instance = nullptr;

ScriptingEntities::~ScriptingEntities() {}

void ScriptingEntities::bind(SLB::Manager* manager) {
	assert(instance);
	manager->set("spawnEntityAt", SLB::FuncCall::create(ScriptingEntities::spawnEntityAt));
	manager->set("spawnGolemAt", SLB::FuncCall::create(ScriptingEntities::spawnGolemAt));
	manager->set("spawnGolem", SLB::FuncCall::create(ScriptingEntities::spawnGolem));
	manager->set("spawnBallAt", SLB::FuncCall::create(ScriptingEntities::spawnBallAt));
	manager->set("spawnBall", SLB::FuncCall::create(ScriptingEntities::spawnBall));
	manager->set("spawnMedusaAt", SLB::FuncCall::create(ScriptingEntities::spawnMedusaAt));
	manager->set("spawnMedusa", SLB::FuncCall::create(ScriptingEntities::spawnMedusa));
	manager->set("stopEntities", SLB::FuncCall::create(ScriptingEntities::stopEntities));
	manager->set("resumeEntities", SLB::FuncCall::create(ScriptingEntities::resumeEntities));
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
	int spawnCounter = get()->spawnCounters[nameComp->getName()];
	std::string name = "s" + std::string(nameComp->getName()) + std::to_string(spawnCounter);
	get()->spawnCounters[nameComp->getName()] = spawnCounter + 1;
	nameComp->setName(name.c_str());
	return name;
}

std::string ScriptingEntities::spawnGolemAt(float x, float y, float z) {
	return spawnEntityAt(PREFAB_GOLEM, x, y, z);
}

std::string ScriptingEntities::spawnGolem() {
	VEC3 spawnPosition = get()->getPlayerTransform()->getPosition() + (get()->getPlayerTransform()->getFront() * 2);
	return spawnGolemAt(spawnPosition.x, spawnPosition.y, spawnPosition.z);
}

std::string ScriptingEntities::spawnBallAt(float x, float y, float z) {
	return spawnEntityAt(PREFAB_BALL, x, y, z);
}

std::string ScriptingEntities::spawnBall() {
	VEC3 spawnPosition = get()->getPlayerTransform()->getPosition() + (get()->getPlayerTransform()->getFront() * 2);
	return spawnBallAt(spawnPosition.x, spawnPosition.y, spawnPosition.z);
}

std::string ScriptingEntities::spawnMedusaAt(float x, float y, float z) {
	return spawnEntityAt(PREFAB_MEDUSA, x, y, z);
}

std::string ScriptingEntities::spawnMedusa() {
	VEC3 spawnPosition = get()->getPlayerTransform()->getPosition() + (get()->getPlayerTransform()->getFront() * 2);
	spawnPosition.y += 4.f;
	return spawnMedusaAt(spawnPosition.x, spawnPosition.y, spawnPosition.z);
}

TCompTransform* ScriptingEntities::getPlayerTransform() {
	CEntity* playerEntity = getEntityByName(PLAYER_NAME);
	assert(playerEntity);
	return playerEntity->get<TCompTransform>();
}

void ScriptingEntities::stopEntities() {
	Engine.getEntities().setManagerUpdate("skeleton", false);
	Engine.getEntities().setManagerUpdate("behaviorTree_melee_enemy", false);
	Engine.getEntities().setManagerUpdate("behaviorTree_ball_enemy", false);
	Engine.getEntities().setManagerUpdate("behaviorTree_flying_ranged_enemy", false);
	Engine.getEntities().setManagerUpdate("player_controller", false);
	Engine.getEntities().setManagerUpdate("ranged_attack", false);
	Engine.getEntities().setManagerUpdate("power_gauge", false);
	Engine.getEntities().setManagerUpdate("platform_move", false);
	Engine.getEntities().setManagerUpdate("platform_rotative", false);
	Engine.getEntities().setManagerUpdate("follow_curve", false);
}

void ScriptingEntities::resumeEntities() {
	Engine.getEntities().setManagerUpdate("skeleton", true);
	Engine.getEntities().setManagerUpdate("behaviorTree_melee_enemy", true);
	Engine.getEntities().setManagerUpdate("behaviorTree_ball_enemy", true);
	Engine.getEntities().setManagerUpdate("behaviorTree_flying_ranged_enemy", true);
	Engine.getEntities().setManagerUpdate("player_controller", true);
	Engine.getEntities().setManagerUpdate("ranged_attack", true);
	Engine.getEntities().setManagerUpdate("power_gauge", true);
	Engine.getEntities().setManagerUpdate("platform_move", true);
	Engine.getEntities().setManagerUpdate("platform_rotative", true);
	Engine.getEntities().setManagerUpdate("follow_curve", true);
}