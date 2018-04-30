#include "mcv_platform.h"
#include "scripting_manager.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "components/comp_name.h"
#include "entity/entity_parser.h"
#include <SLB/SLB.hpp>
#include "components/player/comp_player_model.h"

ScriptingManager* ScriptingManager::instance = nullptr;

ScriptingManager::ScriptingManager() {
}

ScriptingManager::~ScriptingManager() {
}


void ScriptingManager::bind(SLB::Manager* manager) {
	assert(instance);
	manager->set("callDelayed", SLB::FuncCall::create(ScriptingManager::callDelayed));
	manager->set("setPlayerHp", SLB::FuncCall::create(ScriptingManager::setPlayerHp));
}

void ScriptingManager::callDelayed(float delay, const char* func, const char* params) {
	Engine.getScripting().callDelayed(delay, func, params);
}

void ScriptingManager::setPlayerHp(float hp) {
	CEntity* playerEntity = getEntityByName(PLAYER_NAME);
	TCompPlayerModel *playerModel = playerEntity->get<TCompPlayerModel>();
	playerModel->setHp(hp);
}