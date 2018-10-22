#include "mcv_platform.h"
#include "comp_collectable_manager.h"
#include "utils/utils.h"
#include "entity/common_msgs.h"
#include "comp_player_model.h"
#include "components/comp_dummy_collectable.h"
#include "modules/game_modules/game/respawner.h"
#include "modules/game_modules/game/module_game.h"
#include <algorithm>

DECL_OBJ_MANAGER("collectable_manager", TCompCollectableManager);

using Type = TCompCollectable::Type;

void TCompCollectableManager::registerMsgs() {
	DECL_MSG(TCompCollectableManager, TMsgCollect, onCollect);
	DECL_MSG(TCompCollectableManager, TMsgEntityCreated, onEntityCreated);
	DECL_MSG(TCompCollectableManager, TMsgAllScenesCreated, onAllScenesCreated);
}

void TCompCollectableManager::load(const json & j, TEntityParseContext & ctx) {
	if (j.count("final_door_chrysalides")) {
		auto& chrysalides = j["final_door_chrysalides"];
		assert(chrysalides.is_array());
		for (std::string chrysalis : chrysalides) {
			finalDoorChrysalidesNames.push_back(chrysalis);
		}
	}
	finalDoorControllerName = j.value("final_door_controller", "");
}

void TCompCollectableManager::onAllScenesCreated(const TMsgAllScenesCreated & msg) {
	for (auto& name : finalDoorChrysalidesNames) {
		CEntity* entity = getEntityByName(name);
		CHandle handle = entity->get<TCompRender>();
		finalDoorChrysalides.push_back(handle);
	}
	finalDoorControllerEntity = getEntityByName(finalDoorControllerName);
}

void TCompCollectableManager::update(float delta) {
	if (spawnDoorChrysalis) {
		if (doorChrysalidesTimer.elapsed() >= doorChrysalidesTime) {
			((TCompRender*)finalDoorChrysalides[numberOfChrysalisTaken - 1])->enable();
			spawnDoorChrysalis = false;
		}
	}
	if (isCollecting) {
		if (collectTimer.elapsed() >= collectDuration) {
			TCompPlayerModel* playerModel = get<TCompPlayerModel>();
			playerModel->changeState("AirborneNormal");
			isCollecting = false;
			CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
			TCompCameraPlayer* cameraPlayer = playerCameraEntity->get<TCompCameraPlayer>();
			cameraPlayer->resetSuggested();
			playerModel->setHp(playerModel->getMaxHp());
		}
	}
}

void TCompCollectableManager::debugInMenu() {
	float collectPitchDegrees = rad2deg(collectPitch);
	if (ImGui::DragFloat("Collect Pitch", &collectPitchDegrees, 1.f, -80.f, -10.f)) {
		collectPitch = deg2rad(collectPitchDegrees);
	}

	float collectYawDegrees = rad2deg(collectYaw);
	if (ImGui::DragFloat("Collect Yaw", &collectYawDegrees, 1.f, -90.f, 90.f)) {
		collectYaw = deg2rad(collectYawDegrees);
	}

	ImGui::DragFloat("spawn door chrysalis time", &doorChrysalidesTime, 0.05f, 0.f, 10.f);

	//collectYaw
	ImGui::DragFloat2("Camera Speed", &cameraSpeed.x, 0.1f, 0.1f, 5.f);
	ImGui::DragFloat("Collect Duration", &collectDuration, 0.1f, 1.f, 20.f);
	ImGui::DragFloat("Collect Distance", &collectDistance, 0.1f, 0.5f, 20.f);
}

void TCompCollectableManager::addUniqueCollectable(Type type, std::string id) {
	if (uniqueObjectsCollected.find(type) != uniqueObjectsCollected.end()) {
		addCollectable(type, 1);
		//llamar a module uniques si el tipo es coin, chrysalis...
		bool isUnique = false;
		switch (type) {
		case Type::CHRYSALIS:
			isUnique = EngineUniques.setChrysalisTaken(id, true);
			break;
		case Type::COIN:
			isUnique = EngineUniques.setCoinTaken(id, true);
			break;
		case Type::LIFEPIECE:
			isUnique = EngineUniques.setLifePieceTaken(id, true);
			break;
		}
		if (isUnique) {
			uniqueObjectsCollected[type].push_back(id);
		}
	}
}

void TCompCollectableManager::addCollectable(Type type, int amount) {
	objectsCollected[type] += amount;
}

int TCompCollectableManager::getNumberOfChrysalis() {
	return objectsCollected[Type::CHRYSALIS];
}

int TCompCollectableManager::getNumberOfCoins() {
	return objectsCollected[Type::COIN];
}

int TCompCollectableManager::getMaxCoins() {
	return maxCoins;
}

int TCompCollectableManager::getNumberOfLifePieces() {
	return objectsCollected[Type::LIFEPIECE];
}

bool TCompCollectableManager::spendCoins(int number) {
	if (number > objectsCollected[Type::COIN]) {
		return false;
	}
	objectsCollected[Type::COIN] -= number;

	//Avisar a ModuleUniques
	int nUniquesToErase = std::min(number, (int)uniqueObjectsCollected[Type::COIN].size());
	if (nUniquesToErase > 0) {
		for (int i = 0; i < nUniquesToErase; i++) {
			UniqueElement* element = EngineUniques.getUniqueCoin(uniqueObjectsCollected[Type::COIN][i]);
			((CModuleGame*)(EngineModules.getModule("game")))->getRespawner()->addElementToSpawn(
				uniqueObjectsCollected[Type::COIN][i],
				PREFAB_COIN,
				element->position,
				timeToRespawnCoin
			);
			EngineUniques.setCoinTaken(uniqueObjectsCollected[Type::COIN][i], false);
		}
		uniqueObjectsCollected[Type::COIN].erase(uniqueObjectsCollected[Type::COIN].begin(), uniqueObjectsCollected[Type::COIN].begin() + nUniquesToErase);
	}

	return true;
}

void TCompCollectableManager::clear() {
	objectsCollected[Type::CHRYSALIS] = 0;
	objectsCollected[Type::COIN] = 0;
	objectsCollected[Type::LIFEPIECE] = 0;
	uniqueObjectsCollected[Type::CHRYSALIS].clear();
	uniqueObjectsCollected[Type::COIN].clear();
	uniqueObjectsCollected[Type::LIFEPIECE].clear();
}

void TCompCollectableManager::onEntityCreated(const TMsgEntityCreated & msg) {
	clear();
}

void TCompCollectableManager::onCollect(const TMsgCollect& msg) {
	TCompCollectable* collectable = msg.collectableHandle;
	CEntity* entity = msg.collectableHandle.getOwner();
	TCompPlayerModel* playerModel = get<TCompPlayerModel>();
	TCompTransform* transform = entity->get<TCompTransform>();
	TCompTransform* playerTransform = get<TCompTransform>();
	CEntity* playerCameraEntity = getEntityByName(PLAYER_CAMERA);
	TCompCameraPlayer* cameraPlayer = playerCameraEntity->get<TCompCameraPlayer>();
	float yaw, pitch;

	if (msg.type != Type::COIN) {
		playerModel->setDummyState("get_chrysalis", false, collectDuration, "idle_SS3");
		playerModel->changeState("Dummy");
		//situar c�mara frente a player
		playerTransform->getYawPitchRoll(&yaw, &pitch);
		yaw += M_PI + deg2rad(collectYaw);
		//TODO if camera is already suggested, remember parameters
		cameraPlayer->suggestYawPitchDistance(yaw, collectPitch, collectDistance, true, false, true, true, false);
		cameraPlayer->placeCameraOnSuggestedPosition(cameraSpeed);
		isCollecting = true;
		collectTimer.reset();
	}

	switch (msg.type) {
	case Type::CHRYSALIS:
		numberOfChrysalisTaken++;
		if (numberOfChrysalisTaken == 1) {
			EngineScripting.throwEvent(firstChrysalisTaken, "");
		}
		else if(numberOfChrysalisTaken == CHRYSALIS_TARGET_NUMBER){
			EngineScripting.throwEvent(lastChrysalisTaken, "");
			//activar final door trigger
			TMsgChrysalisCollected msgChrysalisCollected = { numberOfChrysalisTaken };
			CEntity* entity = finalDoorControllerEntity;
			entity->sendMsg(msgChrysalisCollected);
		}
		if (numberOfChrysalisTaken <= CHRYSALIS_TARGET_NUMBER) {
			//esperar 4 segundos y hacer aparecer chrysalis (final_door_chrysalis_X)
			spawnDoorChrysalis = true;
			doorChrysalidesTimer.reset();
		}

		collectable->collect();
		addUniqueCollectable(Type::CHRYSALIS, entity->getName());
		EngineSound.emitEvent(SOUND_COLLECT_CHRYSALIS, transform);
		EngineSound.emitEvent(SOUND_HEAL);
		(static_cast<TCompDummyCollectable*>(get<TCompDummyCollectable>()))->activateSequence(DummyCollectableType::CHRYSALIS);
		//Esto de aqu� molar�a no hacerlo el mismo frame en que recoges el objeto 
		//playerModel->setHp(playerModel->getMaxHp());
		EngineGame->showChrysalis(showChrysalisTime);
		break;
	case Type::COIN:
		collectable->collect();
		addUniqueCollectable(Type::COIN, entity->getName());
		if (getNumberOfCoins() > maxCoins) {
			spendCoins(getNumberOfCoins() - maxCoins);
		}
		EngineParticles.launchSystem(PARTICLES_COIN, { entity });
		EngineSound.emitEvent(SOUND_COLLECT_COIN, transform);
		break;
	case Type::LIFEPIECE:
		collectable->collect();
		addUniqueCollectable(Type::LIFEPIECE, entity->getName());
		(static_cast<TCompDummyCollectable*>(get<TCompDummyCollectable>()))->activateSequence(DummyCollectableType::LIFEPIECE);
		if (getNumberOfLifePieces() % lifePiecesPerHeart == 0) {
			playerModel->setMaxHp(playerModel->getMaxHp() + 1);
			//playerModel->setHp(playerModel->getMaxHp());
		}
		EngineSound.emitEvent(SOUND_HEAL);
		break;
	default:
		dbg("Collected unknown object %d\n", msg.type);
		break;
	}
}

