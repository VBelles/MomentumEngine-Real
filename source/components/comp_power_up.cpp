#include "mcv_platform.h"
#include "comp_power_up.h"
#include "components/player/comp_player_model.h"
#include "components/comp_dummy_collectable.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("power_up", TCompPowerUp);

void TCompPowerUp::debugInMenu() {
	ImGui::Text("I give you: %s", stateToUnlock.c_str());
}

void TCompPowerUp::registerMsgs() {
	DECL_MSG(TCompPowerUp, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompPowerUp, TMsgAllScenesCreated, onAllScenesCreated);
	DECL_MSG(TCompPowerUp, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompPowerUp, TMsgColliderDestroyed, onColliderDestroyed);
}

void TCompPowerUp::load(const json& j, TEntityParseContext& ctx) {
	stateToUnlock = j.value("state_to_unlock","");
	message = j.value("message", "You unlocked a new ability!");
	rotationSpeed = j.value("rotation_speed", rotationSpeed);
}

void TCompPowerUp::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	transformHandle = get<TCompTransform>();
	assert(transformHandle.isValid());
}

void TCompPowerUp::onAllScenesCreated(const TMsgAllScenesCreated & msg) {
	CEntity* player = getEntityByName(PLAYER_NAME);
	playerModelHandle = player->get<TCompPlayerModel>();
	assert(playerModelHandle.isValid());
	playerTransformHandle = player->get<TCompTransform>();
	assert(playerTransformHandle.isValid());
}

void TCompPowerUp::update(float delta) {
	if (abs(rotationSpeed) > 0) {
		TCompTransform* transform = getTransform();
		VEC3 rotationAxis = VEC3(0, 1, 0);
		QUAT quat = QUAT::CreateFromAxisAngle(rotationAxis, rotationSpeed * delta);
		transform->setRotation(transform->getRotation() * quat);
	}
	if (isCollecting) {
		if (collectTimer.elapsed() >= collectDuration) {
			getPlayerModel()->changeState("AirborneNormal");
			isCollecting = false;
			CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
			TCompCameraPlayer* cameraPlayer = playerCameraEntity->get<TCompCameraPlayer>();
			cameraPlayer->resetSuggested();
			CHandle(this).getOwner().destroy();
		}
	}
}

void TCompPowerUp::onTriggerEnter(const TMsgTriggerEnter & msg) {
	CEntity* entity = msg.h_other_entity;
	if (entity->getName() == PLAYER_NAME) {
		//unlock state in player manager through player model
		((TCompPlayerModel*)getPlayerModel())->unlockState(stateToUnlock);

		std::string name = ((CEntity*)CHandle(this).getOwner())->getName();
		UniquePowerUp* uniquePowerUp = EngineUniques.getUniquePowerUp(name);
		if (uniquePowerUp && !uniquePowerUp->done) {
			uniquePowerUp->done = true;
		}
		((TCompRender*)get<TCompRender>())->disable();
		((TCompCollider*)get<TCompCollider>())->destroy();

		//cutscene
		CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
		TCompCameraPlayer* cameraPlayer = playerCameraEntity->get<TCompCameraPlayer>();
		TCompPlayerModel* playerModel = getPlayerModel();
		float yaw, pitch;
		playerModel->setDummyState("get_chrysalis", false, collectDuration, "idle_SS3");
		playerModel->changeState("Dummy");
		//situar cámara frente a player
		getPlayerTransform()->getYawPitchRoll(&yaw, &pitch);
		yaw += M_PI + deg2rad(collectYaw);;
		//if camera is already suggested, remember parameters
		cameraPlayer->suggestYawPitchDistance(yaw, collectPitch, collectDistance, true, false, true, true, false);
		cameraPlayer->placeCameraOnSuggestedPosition(cameraSpeed);
		(static_cast<TCompDummyCollectable*>(entity->get<TCompDummyCollectable>()))->activateSequence(DummyCollectableType::POWERUP);

		isCollecting = true;
		collectTimer.reset();
		playerModel->setHp(playerModel->getMaxHp());

		EngineGUI.showDialog(message, 16);
	}
}

void TCompPowerUp::onColliderDestroyed(const TMsgColliderDestroyed& msg) {
	//hacer esto al final de la secuencia
	//CHandle(this).getOwner().destroy();
}

TCompTransform* TCompPowerUp::getTransform() {
	return transformHandle;
}

TCompPlayerModel* TCompPowerUp::getPlayerModel() {
	return playerModelHandle;
}

TCompTransform * TCompPowerUp::getPlayerTransform() {
	return playerTransformHandle;
}
