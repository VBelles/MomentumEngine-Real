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
	manager->set("PREFAB_GOLEM", SLB::Value::copy(PREFAB_GOLEM));
	manager->set("PREFAB_BALL", SLB::Value::copy(PREFAB_BALL));
	manager->set("PREFAB_MEDUSA", SLB::Value::copy(PREFAB_MEDUSA));
	manager->set("PREFAB_CHRYSALIS", SLB::Value::copy(PREFAB_CHRYSALIS));

	manager->set("INTERPOLATOR_LINEAR", SLB::Value::copy(INTERPOLATOR_LINEAR));
	manager->set("INTERPOLATOR_QUAD_IN", SLB::Value::copy(INTERPOLATOR_QUAD_IN));
	manager->set("INTERPOLATOR_QUAD_OUT", SLB::Value::copy(INTERPOLATOR_QUAD_OUT));
	manager->set("INTERPOLATOR_QUAD_IN_OUT", SLB::Value::copy(INTERPOLATOR_QUAD_IN_OUT));
	manager->set("INTERPOLATOR_CUBIC_IN", SLB::Value::copy(INTERPOLATOR_CUBIC_IN));
	manager->set("INTERPOLATOR_CUBIC_OUT", SLB::Value::copy(INTERPOLATOR_CUBIC_OUT));
	manager->set("INTERPOLATOR_CUBIC_IN_OUT", SLB::Value::copy(INTERPOLATOR_CUBIC_IN_OUT));
	manager->set("INTERPOLATOR_QUART_IN", SLB::Value::copy(INTERPOLATOR_QUART_IN));
	manager->set("INTERPOLATOR_QUART_OUT", SLB::Value::copy(INTERPOLATOR_QUART_OUT));
	manager->set("INTERPOLATOR_QUART_IN_OUT", SLB::Value::copy(INTERPOLATOR_QUART_IN_OUT));
	manager->set("INTERPOLATOR_QUINT_IN", SLB::Value::copy(INTERPOLATOR_QUINT_IN));
	manager->set("INTERPOLATOR_QUINT_OUT", SLB::Value::copy(INTERPOLATOR_QUINT_OUT));
	manager->set("INTERPOLATOR_QUINT_IN_OUT", SLB::Value::copy(INTERPOLATOR_QUINT_IN_OUT));
	manager->set("INTERPOLATOR_BACK_IN", SLB::Value::copy(INTERPOLATOR_BACK_IN));
	manager->set("INTERPOLATOR_BACK_OUT", SLB::Value::copy(INTERPOLATOR_BACK_OUT));
	manager->set("INTERPOLATOR_BACK_IN_OUT", SLB::Value::copy(INTERPOLATOR_BACK_IN_OUT));
	manager->set("INTERPOLATOR_ELASTIC_IN", SLB::Value::copy(INTERPOLATOR_ELASTIC_IN));
	manager->set("INTERPOLATOR_ELASTIC_OUT", SLB::Value::copy(INTERPOLATOR_ELASTIC_OUT));
	manager->set("INTERPOLATOR_ELASTIC_IN_OUT", SLB::Value::copy(INTERPOLATOR_ELASTIC_IN_OUT));
	manager->set("INTERPOLATOR_BOUNCE_IN", SLB::Value::copy(INTERPOLATOR_BOUNCE_IN));
	manager->set("INTERPOLATOR_BOUNCE_OUT", SLB::Value::copy(INTERPOLATOR_BOUNCE_OUT));
	manager->set("INTERPOLATOR_BOUNCE_IN_OUT", SLB::Value::copy(INTERPOLATOR_BOUNCE_IN_OUT));
	manager->set("INTERPOLATOR_CIRCULAR_IN", SLB::Value::copy(INTERPOLATOR_CIRCULAR_IN));
	manager->set("INTERPOLATOR_CIRCULAR_OUT", SLB::Value::copy(INTERPOLATOR_CIRCULAR_OUT));
	manager->set("INTERPOLATOR_CIRCULAR_IN_OUT", SLB::Value::copy(INTERPOLATOR_CIRCULAR_IN_OUT));
	manager->set("INTERPOLATOR_EXPO_IN", SLB::Value::copy(INTERPOLATOR_EXPO_IN));
	manager->set("INTERPOLATOR_EXPO_OUT", SLB::Value::copy(INTERPOLATOR_EXPO_OUT));
	manager->set("INTERPOLATOR_EXPO_IN_OUT", SLB::Value::copy(INTERPOLATOR_EXPO_IN_OUT));
	manager->set("INTERPOLATOR_SINE_IN", SLB::Value::copy(INTERPOLATOR_SINE_IN));
	manager->set("INTERPOLATOR_SINE_OUT", SLB::Value::copy(INTERPOLATOR_SINE_OUT));
	manager->set("INTERPOLATOR_SINE_IN_OUT", SLB::Value::copy(INTERPOLATOR_SINE_IN_OUT));

	manager->set("CAMERA_PRIORITY_DEFAULT", SLB::Value::copy(static_cast<int>(CModuleCameras::EPriority::DEFAULT)));
	manager->set("CAMERA_PRIORITY_GAMEPLAY", SLB::Value::copy(static_cast<int>(CModuleCameras::EPriority::GAMEPLAY)));
	manager->set("CAMERA_PRIORITY_TEMPORARY", SLB::Value::copy(static_cast<int>(CModuleCameras::EPriority::TEMPORARY)));
	manager->set("CAMERA_PRIORITY_DEBUG", SLB::Value::copy(static_cast<int>(CModuleCameras::EPriority::DEBUG)));
}

void ScriptingManager::callDelayed(float delay, const char* func, const char* params) {
	EngineScripting.callDelayed(delay, func, params);
}