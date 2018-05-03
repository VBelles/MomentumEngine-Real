#include "mcv_platform.h"
#include "scripting_cameras.h"
#include <SLB/SLB.hpp>

void ScriptingCameras::bind(SLB::Manager* manager) {
	manager->set("blendInCamera", SLB::FuncCall::create(ScriptingCameras::blendInCamera));
}

void ScriptingCameras::blendInCamera(std::string cameraName, float timeToMixIn, int priority, std::string interpolator) {
	CHandle h_camera = getEntityByName(cameraName);
	CModuleCameras::EPriority prio = static_cast<CModuleCameras::EPriority>(priority);
	Interpolator::IInterpolator* interp = Engine.getCameras().getInterpolator(interpolator);
	Engine.getCameras().blendInCamera(h_camera, timeToMixIn, prio, interp);
}