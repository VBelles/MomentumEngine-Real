#include "mcv_platform.h"
#include "scripting_cameras.h"
#include "components/comp_transform.h"
#include <SLB/SLB.hpp>

void ScriptingCameras::bind(SLB::Manager* manager) {
    manager->set("blendInCamera", SLB::FuncCall::create(ScriptingCameras::blendInCamera));
    manager->set("copyRotationFromCamToCam", SLB::FuncCall::create(ScriptingCameras::copyRotationFromCamToCam));
}

void ScriptingCameras::blendInCamera(std::string cameraName, float timeToMixIn, int priority, std::string interpolator) {
	CHandle h_camera = getEntityByName(cameraName);
	CModuleCameras::EPriority prio = static_cast<CModuleCameras::EPriority>(priority);
	Interpolator::IInterpolator* interp = Engine.getCameras().getInterpolator(interpolator);
	Engine.getCameras().blendInCamera(h_camera, timeToMixIn, prio, interp);
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
