#include "mcv_platform.h"
#include "scripting_cameras.h"
#include <SLB/SLB.hpp>
#include "components/postfx/comp_screen_transition.h"

void ScriptingCameras::bind(SLB::Manager* manager) {
	bindConstants(manager);
    manager->set("blendInCamera", SLB::FuncCall::create(ScriptingCameras::blendInCamera));
    manager->set("copyRotationFromCamToCam", SLB::FuncCall::create(ScriptingCameras::copyRotationFromCamToCam));
    manager->set("startScreenTransition", SLB::FuncCall::create(ScriptingCameras::startScreenTransition));
}

void ScriptingCameras::bindConstants(SLB::Manager* manager) {
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

void ScriptingCameras::blendInCamera(std::string cameraName, float timeToMixIn, int priority, std::string interpolator) {
	CHandle h_camera = getEntityByName(cameraName);
	CModuleCameras::EPriority prio = static_cast<CModuleCameras::EPriority>(priority);
	Interpolator::IInterpolator* interp = EngineCameras.getInterpolator(interpolator);
	EngineCameras.blendInCamera(h_camera, timeToMixIn, prio, interp);
}

void ScriptingCameras::copyRotationFromCamToCam(std::string copyCamName, std::string pasteCamName) {
    CEntity* copyCamEntity = getEntityByName(copyCamName);
    CEntity* pasteCamEntity = getEntityByName(pasteCamName);

    TCompTransform* copyCamTransform = copyCamEntity->get<TCompTransform>();
    TCompTransform* pasteCamTransform = pasteCamEntity->get<TCompTransform>();
    float yCopy, pCopy, rCopy;
    copyCamTransform->getYawPitchRoll(&yCopy, &pCopy, &rCopy);
    pasteCamTransform->setYawPitchRoll(yCopy, pCopy, rCopy);
}

void ScriptingCameras::startScreenTransition(float startingRatio, float finalRatio, float startingAlpha, float finalAlpha, float delay) {
	CEntity* playerCameraEntity = getEntityByName(GAME_CAMERA);
	TCompScreenTransition* comp = playerCameraEntity->get<TCompScreenTransition>();
	comp->startTransition(startingRatio, finalRatio, startingAlpha, finalAlpha, delay);
}
