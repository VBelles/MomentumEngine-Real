#include "mcv_platform.h"
#include "scripting_manager.h"
#include <SLB/SLB.hpp>

void ScriptingManager::bind(SLB::Manager* manager) {
	bindConstants(manager);
	manager->set("callDelayed", SLB::FuncCall::create(ScriptingManager::callDelayed));
}

void ScriptingManager::bindConstants(SLB::Manager* manager) {
	manager->set("PLAYER_NAME", SLB::Value::copy(PLAYER_NAME));
	manager->set("PLAYER_CAMERA", SLB::Value::copy(PLAYER_CAMERA));
	manager->set("GAME_CAMERA", SLB::Value::copy(GAME_CAMERA));
}

void ScriptingManager::callDelayed(float delay, const char* func, const char* params) {
	EngineScripting.callDelayed(delay, func, params);
}
