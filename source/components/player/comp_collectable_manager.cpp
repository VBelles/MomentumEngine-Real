#include "mcv_platform.h"
#include "comp_collectable_manager.h"
#include "utils/utils.h"
#include "entity/common_msgs.h"
#include "comp_player_model.h"

DECL_OBJ_MANAGER("collectable_manager", TCompCollectableManager);

//esto es viable?
//#define COIN  TCompCollectable::Type::COIN;

void TCompCollectableManager::registerMsgs() {
	DECL_MSG(TCompCollectableManager, TMsgCollect, onCollect);
	DECL_MSG(TCompCollectableManager, TMsgEntityCreated, onEntityCreated);
}

void TCompCollectableManager::load(const json & j, TEntityParseContext & ctx) {
}

void TCompCollectableManager::update(float delta) {
}

void TCompCollectableManager::addUniqueCollectable(TCompCollectable::Type type, std::string id) {
	if (uniqueObjectsCollected.find(type) != uniqueObjectsCollected.end()) {
		uniqueObjectsCollected[type].push_back(id);
		addCollectable(type, 1);
		//llamar a module uniques si el tipo es coin, chrysalis...
		switch (type) {
		case TCompCollectable::Type::CHRYSALIS:
			EngineUniques.setChrysalisTaken(id, true);
			break;
		case TCompCollectable::Type::COIN:
			EngineUniques.setCoinTaken(id, true);
			break;
		}
	}
}

void TCompCollectableManager::addCollectable(TCompCollectable::Type type, int amount) {
	objectsCollected[type] += amount;
}

int TCompCollectableManager::getNumberOfChrysalis() {
	return objectsCollected[TCompCollectable::Type::CHRYSALIS];
}

int TCompCollectableManager::getNumberOfCoins() {
	return objectsCollected[TCompCollectable::Type::COIN];
}

void TCompCollectableManager::spendCoins(int number) {
	objectsCollected[TCompCollectable::Type::COIN] -= number;
	//Avisar a ModuleUniques
	for (int i = 0; i < number; i++) {
		EngineUniques.setCoinTaken(uniqueObjectsCollected[TCompCollectable::Type::COIN][i], false);
	}
	uniqueObjectsCollected[TCompCollectable::Type::COIN].erase(uniqueObjectsCollected[TCompCollectable::Type::COIN].begin(), uniqueObjectsCollected[TCompCollectable::Type::COIN].begin() + number - 1);
	//Faltaria avisar a quién tenga que respawnearlos

}

void TCompCollectableManager::clear() {
	objectsCollected[TCompCollectable::Type::CHRYSALIS] = 0;
	objectsCollected[TCompCollectable::Type::COIN] = 0;
	uniqueObjectsCollected[TCompCollectable::Type::CHRYSALIS].clear();
	uniqueObjectsCollected[TCompCollectable::Type::COIN].clear();
}


void TCompCollectableManager::onEntityCreated(const TMsgEntityCreated & msg) {
	clear();
}

void TCompCollectableManager::onCollect(const TMsgCollect& msg) {
	TCompCollectable* collectable = msg.collectableHandle;
	CEntity* entity = msg.collectableHandle.getOwner();
	switch (msg.type) {
	case TCompCollectable::Type::CHRYSALIS:
		collectable->collect();
		uniqueObjectsCollected[TCompCollectable::Type::CHRYSALIS].push_back(entity->getName());
		objectsCollected[TCompCollectable::Type::CHRYSALIS]++;
		break;
	case TCompCollectable::Type::COIN:
		collectable->collect();
		uniqueObjectsCollected[TCompCollectable::Type::COIN].push_back(entity->getName());
		objectsCollected[TCompCollectable::Type::COIN]++;
		break;
	default:
		dbg("Collected unknown object %d\n", msg.type);
		break;
	}
}

