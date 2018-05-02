#include "mcv_platform.h"
#include "scripting_manager.h"
#include <SLB/SLB.hpp>

void ScriptingManager::bind(SLB::Manager* manager) {
	manager->set("callDelayed", SLB::FuncCall::create(ScriptingManager::callDelayed));
}

void ScriptingManager::bindGameConstants(SLB::Manager* manager) {
	manager->set("PLAYER_NAME", SLB::Value::copy(PLAYER_NAME));
	manager->set("PLAYER_CAMERA", SLB::Value::copy(PLAYER_CAMERA));
	manager->set("GAME_CAMERA", SLB::Value::copy(GAME_CAMERA));
	manager->set("GOLEM_PREFAB", SLB::Value::copy(GOLEM_PREFAB));
	manager->set("BALL_PREFAB", SLB::Value::copy(BALL_PREFAB));
	manager->set("MEDUSA_PREFAB", SLB::Value::copy(MEDUSA_PREFAB));
	manager->set("CHRYSALIS_PREFAB", SLB::Value::copy(CHRYSALIS_PREFAB));
	manager->set("INTERPOLATOR_LINEAL", SLB::Value::copy(INTERPOLATOR_LINEAL));
	manager->set("INTERPOLATOR_CUBIC_IN", SLB::Value::copy(INTERPOLATOR_CUBIC_IN));
	manager->set("INTERPOLATOR_CUBIC_OUT", SLB::Value::copy(INTERPOLATOR_CUBIC_OUT));
	manager->set("INTERPOLATOR_CUBIC_IN_OUT", SLB::Value::copy(INTERPOLATOR_CUBIC_IN_OUT));
	manager->set("INTERPOLATOR_EXPO_IN", SLB::Value::copy(INTERPOLATOR_EXPO_IN));
	manager->set("INTERPOLATOR_EXPO_OUT", SLB::Value::copy(INTERPOLATOR_EXPO_OUT));
	manager->set("INTERPOLATOR_EXPO_IN_OUT", SLB::Value::copy(INTERPOLATOR_EXPO_IN_OUT));
	manager->set("CAMERA_PRIORITY_DEFAULT", SLB::Value::copy(static_cast<int>(CModuleCameras::EPriority::DEFAULT)));
	manager->set("CAMERA_PRIORITY_GAMEPLAY", SLB::Value::copy(static_cast<int>(CModuleCameras::EPriority::GAMEPLAY)));
	manager->set("CAMERA_PRIORITY_TEMPORARY", SLB::Value::copy(static_cast<int>(CModuleCameras::EPriority::TEMPORARY)));
	manager->set("CAMERA_PRIORITY_DEBUG", SLB::Value::copy(static_cast<int>(CModuleCameras::EPriority::DEBUG)));
}

void ScriptingManager::callDelayed(float delay, const char* func, const char* params) {
	Engine.getScripting().callDelayed(delay, func, params);
}