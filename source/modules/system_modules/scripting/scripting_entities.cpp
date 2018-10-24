#include "mcv_platform.h"
#include "scripting_entities.h"
#include "components/comp_name.h"
#include "entity/entity_parser.h"
#include <SLB/SLB.hpp>
#include "components/platforms/comp_platforms_director.h"
#include "components/comp_day_night_cycle.h"
#include "components/platforms/comp_platform_simple.h"
#include "components/comp_final_door_controller.h"
#include "modules/game_modules/module_pause.h"
#include "modules/game_modules/module_map_menu.h"

ScriptingEntities* ScriptingEntities::instance = nullptr;

ScriptingEntities::~ScriptingEntities() {}

void ScriptingEntities::bind(SLB::Manager* manager) {
	assert(instance);
	bindConstants(manager);
	manager->set("spawnEntityAt", SLB::FuncCall::create(ScriptingEntities::spawnEntityAt));
	manager->set("spawnEntity", SLB::FuncCall::create(ScriptingEntities::spawnEntity));
	manager->set("spawnGolemAt", SLB::FuncCall::create(ScriptingEntities::spawnGolemAt));
	manager->set("spawnGolem", SLB::FuncCall::create(ScriptingEntities::spawnGolem));
	manager->set("setPlatformEnabled", SLB::FuncCall::create(ScriptingEntities::setPlatformEnabled));
	manager->set("stopEntities", SLB::FuncCall::create(ScriptingEntities::stopEntities));
	manager->set("stopEntitiesCutscene", SLB::FuncCall::create(ScriptingEntities::stopEntitiesCutscene));
	manager->set("resumeEntities", SLB::FuncCall::create(ScriptingEntities::resumeEntities));
	manager->set("resumeEntitiesCutscene", SLB::FuncCall::create(ScriptingEntities::resumeEntitiesCutscene));
	manager->set("stopEnemies", SLB::FuncCall::create(ScriptingEntities::stopEnemies));
	manager->set("resumeEnemies", SLB::FuncCall::create(ScriptingEntities::resumeEnemies));
	manager->set("rotateFinalDoor", SLB::FuncCall::create(ScriptingEntities::rotateFinalDoor));
	manager->set("enableRender", SLB::FuncCall::create(ScriptingEntities::enableRender));
	manager->set("createCollider", SLB::FuncCall::create(ScriptingEntities::createCollider));
	manager->set("destroyCollider", SLB::FuncCall::create(ScriptingEntities::destroyCollider));
	manager->set("blockMenu", SLB::FuncCall::create(ScriptingEntities::blockMenu));
}

void ScriptingEntities::bindConstants(SLB::Manager* manager) {
	manager->set("PREFAB_CHRYSALIS", SLB::Value::copy(PREFAB_CHRYSALIS));
	manager->set("PREFAB_COIN", SLB::Value::copy(PREFAB_COIN));
	manager->set("PREFAB_DREIDEL", SLB::Value::copy(PREFAB_DREIDEL));
	manager->set("PREFAB_GOLEM", SLB::Value::copy(PREFAB_GOLEM));
	manager->set("PREFAB_KIPPAH", SLB::Value::copy(PREFAB_KIPPAH));
	manager->set("PREFAB_DREIDEL_DUMMY", SLB::Value::copy(PREFAB_DREIDEL_DUMMY));
	manager->set("PREFAB_GOLEM_DUMMY", SLB::Value::copy(PREFAB_GOLEM_DUMMY));
	manager->set("PREFAB_KIPPAH_DUMMY", SLB::Value::copy(PREFAB_KIPPAH_DUMMY));
	manager->set("PREFAB_KIPPAH_DUMMY", SLB::Value::copy(PREFAB_KIPPAH_DUMMY));
	manager->set("PREFAB_MURO", SLB::Value::copy(PREFAB_MURO));
	manager->set("PREFAB_PLATAFORMA_HEX", SLB::Value::copy(PREFAB_PLATAFORMA_HEX));
	manager->set("PREFAB_PLATAFORMA_RECT", SLB::Value::copy(PREFAB_PLATAFORMA_RECT));
	manager->set("PREFAB_PLATAFORMA_ROCA", SLB::Value::copy(PREFAB_PLATAFORMA_ROCA));
	manager->set("PREFAB_PRISMA", SLB::Value::copy(PREFAB_PRISMA));
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

std::string ScriptingEntities::spawnEntity(std::string prefabFilename) {
	VEC3 spawnPosition = get()->getPlayerTransform()->getPosition() + (get()->getPlayerTransform()->getFront() * 2);
	return spawnEntityAt(prefabFilename, spawnPosition.x, spawnPosition.y, spawnPosition.z);
}

std::string ScriptingEntities::spawnGolemAt(float x, float y, float z) {
	return spawnEntityAt(PREFAB_GOLEM, x, y, z);
}

std::string ScriptingEntities::spawnGolem() {
	VEC3 spawnPosition = get()->getPlayerTransform()->getPosition() + (get()->getPlayerTransform()->getFront() * 2);
	return spawnGolemAt(spawnPosition.x, spawnPosition.y, spawnPosition.z);
}

void ScriptingEntities::setPlatformEnabled(std::string entityName, bool how) {
	CEntity* entity = getEntityByName(entityName);
	if (entity) {
		TCompPlatformSimple* platfSimple = entity->get<TCompPlatformSimple>();
		if (platfSimple) {
			platfSimple->setEnabled(how);
		}
	}
}

TCompTransform* ScriptingEntities::getPlayerTransform() {
	CEntity* playerEntity = getEntityByName(PLAYER_NAME);
	assert(playerEntity);
	return playerEntity->get<TCompTransform>();
}

void ScriptingEntities::stopEntities() {
	Engine.getEntities().setManagerUpdate("skeleton", false);
	Engine.getEntities().setManagerUpdate("enemy", false);
	Engine.getEntities().setManagerUpdate("player_controller", false);
	Engine.getEntities().setManagerUpdate("ranged_attack", false);
	Engine.getEntities().setManagerUpdate("power_gauge", false);
	Engine.getEntities().setManagerUpdate("follow_curve", false);
	Engine.getEntities().setManagerUpdate("player_model", false);
	Engine.getEntities().setManagerUpdate("platform_simple", false);
	Engine.getEntities().setManagerUpdate("collectable", false);
	Engine.getEntities().setManagerUpdate("platforms_director", false);
	EngineParticles.setPaused(true);
	getObjectManager<TCompPlatformsDirector>()->forEach(
		[](TCompPlatformsDirector* pd) {pd->setEnabled(false); }
	);
	getObjectManager<TCompDayNightCycle>()->forEach(
		[](TCompDayNightCycle* cycle) {cycle->setEnabled(false); }
	);
	cb_globals.game_paused = 1;
	EngineRender.setActive(false);
}

void ScriptingEntities::stopEntitiesCutscene() {
	Engine.getEntities().setManagerUpdate("skeleton", false);
	Engine.getEntities().setManagerUpdate("enemy", false);
	Engine.getEntities().setManagerUpdate("player_controller", false);
	Engine.getEntities().setManagerUpdate("ranged_attack", false);
	Engine.getEntities().setManagerUpdate("power_gauge", false);
	Engine.getEntities().setManagerUpdate("follow_curve", false);
	Engine.getEntities().setManagerUpdate("player_model", false);
	Engine.getEntities().setManagerUpdate("platform_simple", false);
	Engine.getEntities().setManagerUpdate("platforms_director", false);
}

void ScriptingEntities::resumeEntities() {
	Engine.getEntities().setManagerUpdate("skeleton", true);
	Engine.getEntities().setManagerUpdate("enemy", true);
	Engine.getEntities().setManagerUpdate("player_controller", true);
	Engine.getEntities().setManagerUpdate("ranged_attack", true);
	Engine.getEntities().setManagerUpdate("power_gauge", true);
	Engine.getEntities().setManagerUpdate("follow_curve", true);
	Engine.getEntities().setManagerUpdate("player_model", true);
	Engine.getEntities().setManagerUpdate("platform_simple", true);
	Engine.getEntities().setManagerUpdate("collectable", true);
	Engine.getEntities().setManagerUpdate("platforms_director", true);
	EngineParticles.setPaused(false);
	getObjectManager<TCompPlatformsDirector>()->forEach(
		[](TCompPlatformsDirector* pd) {pd->setEnabled(true); }
	);
	getObjectManager<TCompDayNightCycle>()->forEach(
		[](TCompDayNightCycle* cycle) {cycle->setEnabled(true); }
	);
	cb_globals.game_paused = 0;
	EngineRender.setActive(true);
}

void ScriptingEntities::resumeEntitiesCutscene() {
	Engine.getEntities().setManagerUpdate("skeleton", true);
	Engine.getEntities().setManagerUpdate("enemy", true);
	Engine.getEntities().setManagerUpdate("player_controller", true);
	Engine.getEntities().setManagerUpdate("ranged_attack", true);
	Engine.getEntities().setManagerUpdate("power_gauge", true);
	Engine.getEntities().setManagerUpdate("follow_curve", true);
	Engine.getEntities().setManagerUpdate("player_model", true);
	Engine.getEntities().setManagerUpdate("platform_simple", true);
	Engine.getEntities().setManagerUpdate("platforms_director", true);
}

void ScriptingEntities::stopEnemies() {
	Engine.getEntities().setManagerUpdate("enemy", false);
}

void ScriptingEntities::resumeEnemies() {
	Engine.getEntities().setManagerUpdate("enemy", true);
}

void ScriptingEntities::rotateFinalDoor() {
	CEntity* entity = getEntityByName(FINAL_DOOR_CONTROLLER);
	if (entity) {
		TCompFinalDoorController* controller = entity->get<TCompFinalDoorController>();
		if (controller) {
			controller->rotateFinalDoor();
		}
	}
}

void ScriptingEntities::enableRender(std::string entityName, bool enabled) {
	CEntity* entity = getEntityByName(entityName);
	if (entity) {
		TCompRender* render = entity->get<TCompRender>();
		if (render) {
			if (enabled) {
				render->enable();
			}
			else {
				render->disable();
			}
		}
	}
}

void ScriptingEntities::createCollider(std::string entityName) {
	CEntity* entity = getEntityByName(entityName);
	if (entity) {
		TCompCollider* collider = entity->get<TCompCollider>();
		if (collider) {
			dbg("Creating collider %s\n", entityName.c_str());
			collider->create();
		}
		else {
			dbg("Not creating %s (collider not found)\n", entityName.c_str());
		}
	}
	else {
		dbg("Not creating %s (entity not found)\n", entityName.c_str());
	}
}

void ScriptingEntities::destroyCollider(std::string entityName) {
	CEntity* entity = getEntityByName(entityName);
	if (entity) {
		TCompCollider* collider = entity->get<TCompCollider>();
		if (collider) {
			dbg("Destroying collider %s\n", entityName.c_str());
			collider->destroy();
		}
		else {
			dbg("Not destroyed %s (collider not found)\n", entityName.c_str());
		}
	}
	else {
		dbg("Not destroyed %s (entity not found)\n", entityName.c_str());
	}
}

void ScriptingEntities::blockMenu(bool blocked) {
	auto modulePause = (CModulePause*)EngineModules.getModule("pause");
	auto moduleMAp = (CModuleMapMenu*)EngineModules.getModule("map_menu");
	modulePause->setBlocked(blocked);
	moduleMAp->setBlocked(blocked);
}

