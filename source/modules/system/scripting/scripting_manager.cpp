#include "mcv_platform.h"
#include "scripting_manager.h"
#include "components/comp_transform.h"
#include "components/comp_collider.h"
#include "components/comp_name.h"
#include "entity/entity_parser.h"
#include <SLB/SLB.hpp>

ScriptingManager* ScriptingManager::instance = nullptr;

ScriptingManager::ScriptingManager() {
}

ScriptingManager::~ScriptingManager() {
}


void ScriptingManager::bind(SLB::Manager* manager) {
	assert(instance);
	manager->set("callDelayed", SLB::FuncCall::create(ScriptingManager::callDelayed));
}

void ScriptingManager::callDelayed(float delay, const char* call) {
	Engine.getScripting().callDelayed(delay, call);
}