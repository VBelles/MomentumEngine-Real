#include "mcv_platform.h"
#include "comp_mix_camera_trigger.h"
#include "comp_transform.h"
#include "comp_camera.h"
#include "components/comp_collider.h"
#include "components/controllers/comp_camera_curve_interpolation.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("mixCameraTrigger", TCompMixCameraTrigger);

void TCompMixCameraTrigger::debugInMenu() {
}

void TCompMixCameraTrigger::load(const json& j, TEntityParseContext& ctx) {
	assert(j.count("cameraToMix"));
	cameraToMix = j.value("cameraToMix", PLAYER_CAMERA);
	timeToMixIn = j.value("timeToMixIn", 1.f);
	timeToMixOut = j.value("timeToMixOut", 1.f);
	modifyPlayerCameraRotation = j.value("modifyPlayerCameraRotation", true);
}

void TCompMixCameraTrigger::registerMsgs() {
	DECL_MSG(TCompMixCameraTrigger, TMsgEntityCreated, onCreate);
	DECL_MSG(TCompMixCameraTrigger, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompMixCameraTrigger, TMsgTriggerExit, onTriggerExit);
}

void TCompMixCameraTrigger::onCreate(const TMsgEntityCreated& msg) {
	cubicInInterpolator = new TCubicInInterpolator();
	cubicOutInterpolator = new TCubicOutInterpolator();
	cubicInOutInterpolator = new TCubicInOutInterpolator();
	expoInInterpolator = new TExpoInInterpolator();
	expoOutInterpolator = new TExpoOutInterpolator();
	expoInOutInterpolator = new TExpoInOutInterpolator();
}

void TCompMixCameraTrigger::onTriggerEnter(const TMsgTriggerEnter & msg) {
	CEntity* entity = msg.h_other_entity;	
	if (PLAYER_NAME == entity->getName()) {
		CHandle h_camera = getEntityByName(cameraToMix);
		CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
		playerCameraEntity->sendMsg(TMsgLockCameraInput{ true });
		//Mirar ángulo entre fronts
		//Si es mayor de 90, blendea CURVE_INTERPOLATION_CAMERA
		CEntity* cameraToMixEntity = h_camera;
		TCompTransform* playerCameraTransform = playerCameraEntity->get<TCompTransform>();
		TCompTransform* cameraToMixTransform = cameraToMixEntity->get<TCompTransform>();
		VEC2 playerCameraFront2D = VEC2(playerCameraTransform->getFront().x, playerCameraTransform->getFront().z);
		VEC2 cameraToMixFront2D = VEC2(cameraToMixTransform->getFront().x, cameraToMixTransform->getFront().z);
		
		if (playerCameraFront2D.Dot(cameraToMixFront2D) >= 0) {
			Engine.getCameras().blendInCamera(h_camera, timeToMixIn, CModuleCameras::EPriority::GAMEPLAY, cubicOutInterpolator);
		}
		else {
			//Copiar parámetros camera player a curve_interpolation_camera
			CHandle interpolationCameraHandle = getEntityByName(CURVE_INTERPOLATION_CAMERA);
			CEntity* interpolationCameraEntity = interpolationCameraHandle;
			TCompTransform* interpolationCameraTransform = interpolationCameraEntity->get<TCompTransform>();
			TCompCamera* interpolationCamera = interpolationCameraEntity->get<TCompCamera>();
			TCompCamera* playerCamera = playerCameraEntity->get<TCompCamera>();
			interpolationCameraTransform->setPosition(playerCameraTransform->getPosition());
			float y, p;
			playerCameraTransform->getYawPitchRoll(&y, &p);
			interpolationCameraTransform->setYawPitchRoll(y, p);
			interpolationCamera->setPerspective(playerCamera->getFov(), playerCamera->getZNear(), playerCamera->getZFar());
			//interpolationCamera->lookAt(interpolationCameraTransform->getPosition(), interpolationCameraTransform->getPosition() + interpolationCameraTransform->getFront());

			//Blendear en 0 segundos
			Engine.getCameras().blendInCamera(interpolationCameraHandle, 0.001f, CModuleCameras::EPriority::GAMEPLAY);
			
			//Llamar a su función startInterpolation
			TCompCamera* cameraToMix = cameraToMixEntity->get<TCompCamera>();
			TCompCameraCurveInterpolation* interpolationCameraComp = interpolationCameraEntity->get<TCompCameraCurveInterpolation>();
			interpolationCameraComp->startInterpolating(
				h_camera, timeToMixIn, cameraToMixTransform->getPosition(), cameraToMixTransform->getFront(),
				cameraToMix->getFov(), cameraToMix->getZNear(), cameraToMix->getZFar(), cubicOutInterpolator
			);
		}
	}
}

void TCompMixCameraTrigger::onTriggerExit(const TMsgTriggerExit & msg) {

	CEntity* entity = msg.h_other_entity;
	std::string playerName = PLAYER_NAME;
	if (PLAYER_NAME == entity->getName()) {
		//Paramos CURVE_INTERPOLATION_CAMERA
		CEntity* interpolationCameraEntity = getEntityByName(CURVE_INTERPOLATION_CAMERA);
		TCompCameraCurveInterpolation* interpolationCameraComp = interpolationCameraEntity->get<TCompCameraCurveInterpolation>();
		interpolationCameraComp->stopInterpolating();

		CHandle playerCameraHandle = getEntityByName(PLAYER_CAMERA);
		if (!Engine.getCameras().IsCameraAloneInMix(playerCameraHandle)) {
			if (modifyPlayerCameraRotation) {
				CopyRotationFromMixedCameraToPlayerCamera();
				CEntity* cameraToMixEntity = getEntityByName(cameraToMix);
				cameraToMixEntity->sendMsg(TMsgLockCameraInput{ true }); 
				Engine.getCameras().blendInCamera(playerCameraHandle, timeToMixOut, CModuleCameras::EPriority::GAMEPLAY, cubicOutInterpolator);
			}
		}
	}
}

void TCompMixCameraTrigger::CopyRotationFromMixedCameraToPlayerCamera() {
	CHandle leavingCameraHandle = getEntityByName(GAME_CAMERA);
	CEntity* leavingCameraEntity = leavingCameraHandle;
	TCompCamera* leavingCamera = leavingCameraEntity->get<TCompCamera>();
	CHandle playerCameraHandle = getEntityByName(PLAYER_CAMERA);
	CEntity* playerCameraEntity = playerCameraHandle;
	TCompTransform* playerCameraTransform = playerCameraEntity->get<TCompTransform>();
	//playerCameraTransform->setPosition(leavingCameraTransform->getPosition());
	float yPLayer, pPlayer, rPlayer;
	playerCameraTransform->getYawPitchRoll(&yPLayer, &pPlayer, &rPlayer);
	float yLeavingCamera, pLeavingCamera, rLeavingCamera;
	leavingCamera->getYawPitchRoll(&yLeavingCamera, &pLeavingCamera, &rLeavingCamera);
	playerCameraTransform->setYawPitchRoll(yLeavingCamera, pLeavingCamera, rPlayer);
}


