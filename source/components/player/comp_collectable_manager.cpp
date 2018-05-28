#include "mcv_platform.h"
#include "comp_collectable_manager.h"
#include "utils/utils.h"
#include "entity/common_msgs.h"
#include "comp_player_model.h"

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
		uniqueObjectsCollected[type].push_back(id);
		addCollectable(type, 1);
		//llamar a module uniques si el tipo es coin, chrysalis...
		switch (type) {
		case Type::CHRYSALIS:
			EngineUniques.setChrysalisTaken(id, true);
			break;
		case Type::COIN:
			EngineUniques.setCoinTaken(id, true);
			break;
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

void TCompCollectableManager::spendCoins(int number) {
	objectsCollected[Type::COIN] -= number;
	//Avisar a ModuleUniques
	for (int i = 0; i < number; i++) {
		EngineUniques.setCoinTaken(uniqueObjectsCollected[Type::COIN][i], false);
	}
	uniqueObjectsCollected[Type::COIN].erase(uniqueObjectsCollected[Type::COIN].begin(), uniqueObjectsCollected[Type::COIN].begin() + number - 1);
	//Faltaria avisar a quién tenga que respawnearlos

}

void TCompCollectableManager::clear() {
	objectsCollected[Type::CHRYSALIS] = 0;
	objectsCollected[Type::COIN] = 0;
	uniqueObjectsCollected[Type::CHRYSALIS].clear();
	uniqueObjectsCollected[Type::COIN].clear();
}


void TCompCollectableManager::onEntityCreated(const TMsgEntityCreated & msg) {
	clear();
}

void TCompCollectableManager::onCollect(const TMsgCollect& msg) {
	TCompCollectable* collectable = msg.collectableHandle;
	CEntity* entity = msg.collectableHandle.getOwner();
	switch (msg.type) {
	case Type::CHRYSALIS:
		collectable->collect();
		uniqueObjectsCollected[Type::CHRYSALIS].push_back(entity->getName());
		objectsCollected[Type::CHRYSALIS]++;
		break;
	case Type::COIN:
		collectable->collect();
		uniqueObjectsCollected[Type::COIN].push_back(entity->getName());
		objectsCollected[Type::COIN]++;
		break;
	default:
		dbg("Collected unknown object %d\n", msg.type);
		break;
	}
}

