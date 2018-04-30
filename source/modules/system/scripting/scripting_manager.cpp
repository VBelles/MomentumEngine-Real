#include "mcv_platform.h"
#include "scripting_manager.h"
#include <SLB/SLB.hpp>

void ScriptingManager::bind(SLB::Manager* manager) {
	manager->set("callDelayed", SLB::FuncCall::create(ScriptingManager::callDelayed));
}

void ScriptingManager::callDelayed(float delay, const char* func, const char* params) {
	Engine.getScripting().callDelayed(delay, func, params);
}