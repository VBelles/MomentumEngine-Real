#include "mcv_platform.h"
#include "scripting_cameras.h"
#include <SLB/SLB.hpp>

void ScriptingCameras::bind(SLB::Manager* manager) {
	manager->set("blendInCamera", SLB::FuncCall::create(ScriptingCameras::blendInCamera));
}

void ScriptingCameras::blendInCamera(std::string cameraName, float timeToMixIn, int priority, std::string interpolator) {
	CHandle h_camera = getEntityByName(cameraName);
	CModuleCameras::EPriority prio = static_cast<CModuleCameras::EPriority>(priority);

	Interpolator::IInterpolator* interp = nullptr;
	if (interpolator == INTERPOLATOR_LINEAL) { interp = nullptr; }
	else if (interpolator == INTERPOLATOR_CUBIC_IN) { interp = new Interpolator::TCubicInInterpolator(); }
	else if (interpolator == INTERPOLATOR_CUBIC_OUT) { interp = new Interpolator::TCubicOutInterpolator(); }
	else if (interpolator == INTERPOLATOR_CUBIC_IN_OUT) { interp = new Interpolator::TCubicInOutInterpolator(); }
	else if (interpolator == INTERPOLATOR_EXPO_IN) { interp = new Interpolator::TExpoInInterpolator(); }
	else if (interpolator == INTERPOLATOR_EXPO_OUT) { interp = new Interpolator::TExpoOutInterpolator(); }
	else if (interpolator == INTERPOLATOR_EXPO_IN_OUT) { interp = new Interpolator::TExpoInOutInterpolator(); }

	Engine.getCameras().blendInCamera(h_camera, timeToMixIn, prio, interp);
}