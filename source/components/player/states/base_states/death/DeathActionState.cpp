#include "mcv_platform.h"
#include "DeathActionState.h"
#include "components/postfx/comp_screen_transition.h"

DeathActionState::DeathActionState(StateManager* stateManager) :
	IActionState(stateManager, Death) {
}

void DeathActionState::update(float delta) {
	if (timer.elapsed() >= deathTime) {
		//Engine.getEntities().setManagerUpdate("skeleton", false);
	}
	if (timer.elapsed() >= respawnTime) {
		if (!screenTransitionStarted) {
			getScreenTransition()->startTransition(0.f, 1.f);
			screenTransitionStarted = true;
		}
		else {
			if (!getScreenTransition()->isTransitioning()) {
				if (finish) {
					stateManager->changeConcurrentState(Free);
					stateManager->changeState(AirborneNormal);
				}
				else {
					respawn();
					frameCounter = 0;
					finish = true;
					getSkeleton()->blendCycle(animationIdle, 0.0f, 0.0f);
					CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
					EngineCameras.blendInCamera(playerCameraEntity, 0.00001f, CModuleCameras::EPriority::GAMEPLAY);
					getScreenTransition()->startTransition(1.f, 0.f);
				}
			}
		}
	}
	if (timer.elapsed() >= startSelfIllumDecreaseTime) {
		TCompRender* render = getPlayerModel()->get<TCompRender>();
		render->selfIllumRatio = clamp(render->selfIllumRatio - selfIllumDecreaseSpeed * delta, selfIllumMinimumRatio, 1.f);
	}
}

void DeathActionState::onStateEnter(IActionState* lastState) {
	IActionState::onStateEnter(lastState); 
	getSkeleton()->blendCycle(animationLoop, 0.0f, 0.0f);
	getSkeleton()->executeAction(animation, 0.1f, 0.0f);
	getPlayerModel()->setGravity(0);
	timer.reset();
	finish = false;
	EngineScripting.throwEvent(onPlayerKilled, "");

	copyCam();
	CEntity* teleportCameraEntity = getEntityByName(TELEPORT_CAMERA);
	EngineCameras.blendInCamera(teleportCameraEntity, 0.001f, CModuleCameras::EPriority::GAMEPLAY);
	getSound()->play("death");

	screenTransitionStarted = false;
}

void DeathActionState::onStateExit(IActionState* nextState) {
	IActionState::onStateExit(nextState);
	CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
	EngineCameras.blendInCamera(playerCameraEntity, 0.00001f, CModuleCameras::EPriority::GAMEPLAY);
	getPlayerModel()->resetGravity();
	TCompRender* render = getPlayerModel()->get<TCompRender>();
	render->selfIllumRatio = 1.f;
	//Engine.getEntities().setManagerUpdate("skeleton", true);
}

void DeathActionState::respawn() {
	*velocityVector = VEC3();
	assert(getPlayerModel());
	VEC3 respawnPosition = getPlayerModel()->getRespawnPosition();
	TCompTransform* transform = getPlayerTransform();
	float yaw, pitch;
	transform->getYawPitchRoll(&yaw, &pitch);
	transform->setYawPitchRoll(getPlayerModel()->getRespawnYaw(), pitch);
	getCollider()->controller->setFootPosition(PxExtendedVec3(respawnPosition.x, respawnPosition.y, respawnPosition.z));

	CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
	TCompTransform* cameraTransform = playerCameraEntity->get<TCompTransform>();
	TCompCameraPlayer* playerCamera = playerCameraEntity->get<TCompCameraPlayer>();
	cameraTransform->setYawPitchRoll(getPlayerModel()->getRespawnYaw(), playerCamera->getInitialPitch());

	getPlayerModel()->resetHp();
	getPowerGauge()->resetPower();
}

void DeathActionState::copyCam() {
	//presupone que entramos siempre en player camera
	CEntity* copyCamEntity = getEntityByName(PLAYER_CAMERA);
	CEntity* pasteCamEntity = getEntityByName(TELEPORT_CAMERA);

	TCompTransform* copyCamTransform = copyCamEntity->get<TCompTransform>();
	TCompTransform* pasteCamTransform = pasteCamEntity->get<TCompTransform>();
	pasteCamTransform->setPosition(copyCamTransform->getPosition());
	pasteCamTransform->setRotation(copyCamTransform->getRotation());
}

TCompScreenTransition* DeathActionState::getScreenTransition() {
	CEntity* playerCameraEntity = getEntityByName(GAME_CAMERA);
	return playerCameraEntity->get<TCompScreenTransition>();
}