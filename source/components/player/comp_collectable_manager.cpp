#include "mcv_platform.h"
#include "comp_collectable_manager.h"
#include "utils/utils.h"
#include "entity/common_msgs.h"
#include "comp_player_model.h"
#include "components/comp_transform.h"
#include "modules/game_modules/game/respawner.h"
#include "modules/game_modules/game/module_game.h"
#include <algorithm>

DECL_OBJ_MANAGER("collectable_manager", TCompCollectableManager);

using Type = TCompCollectable::Type;

void TCompCollectableManager::registerMsgs() {
	DECL_MSG(TCompCollectableManager, TMsgCollect, onCollect);
	DECL_MSG(TCompCollectableManager, TMsgEntityCreated, onEntityCreated);
}

void TCompCollectableManager::load(const json & j, TEntityParseContext & ctx) {
}

void TCompCollectableManager::update(float delta) {
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
	switch (msg.type) {
	case Type::CHRYSALIS:
		collectable->collect();
		addUniqueCollectable(Type::CHRYSALIS, entity->getName());
		playerModel->setHp(playerModel->getMaxHp());
		EngineSound.emitEvent(SOUND_COLLECT_CHRYSALIS, transform);
		break;
	case Type::COIN:
		collectable->collect();
		addUniqueCollectable(Type::COIN, entity->getName());
		if (getNumberOfCoins() > maxCoins) {
			spendCoins(getNumberOfCoins() - maxCoins);
		}
		EngineParticles.launchSystem(PARTICLES_COIN, { entity });
		EngineSound.emitEvent(SOUND_COLLECT_CHRYSALIS, transform);
		break;
	case Type::LIFEPIECE:
		collectable->collect();
		addUniqueCollectable(Type::LIFEPIECE, entity->getName());
		if (getNumberOfLifePieces() % lifePiecesPerHeart == 0) {
			playerModel->setMaxHp(playerModel->getMaxHp() + 1);
			playerModel->setHp(playerModel->getMaxHp());
		}
		break;
	default:
		dbg("Collected unknown object %d\n", msg.type);
		break;
	}
}

