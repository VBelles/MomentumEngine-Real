#include "mcv_platform.h"
#include "module_uniques.h"
#include "modules/module.h"
#include "entity/entity.h"
#include "modules/system_modules/module_entities.h"
#include "handle/handle.h"
#include "components/comp_name.h"
#include "components/comp_transform.h"
#include "entity/entity_parser.h"


bool CModuleUniques::start() {
	//de momento json, pero debería estar encriptado (haha... lo dudo)
	std::vector<json> jUniques;
	jUniques.push_back(loadJson("data/uniques/unique_coins.json"));
	//jUniques.push_back(loadJson("data/uniques/coins-uniques.json"));//no hace falta que estén en archivos separados
	//jUniques.push_back(loadJson("data/uniques/chrysalides-uniques.json"));
	//jUniques.push_back(loadJson("data/uniques/altars-uniques.json"));
	//jUniques.push_back(loadJson("data/uniques/events-uniques.json"));
	//jUniques.push_back(loadJson("data/uniques/enemies-uniques.json"));

	//parse
	for (auto& jUnique : jUniques) {
		if (jUnique.count("unique_coins")) {
			auto& jCoins = jUnique["unique_coins"];
			if (jCoins.is_array()) {
				for (size_t i = 0; i < jCoins.size(); ++i) {
					parseChunk(jCoins[i], ElementType::COIN);
				}
			}
		}
		if (jUnique.count("unique_chrysalides")) {
			auto& jChrysalides = jUnique["unique_chrysalides"];
			if (jChrysalides.is_array()) {
				for (size_t i = 0; i < jChrysalides.size(); ++i) {
					parseChunk(jChrysalides[i], ElementType::CHRYSALIS);
				}
			}
		}
		if (jUnique.count("unique_altars")) {
			auto& jAltars = jUnique["unique_altars"];
			if (jAltars.is_array()) {
				for (size_t i = 0; i < jAltars.size(); ++i) {
					parseChunk(jAltars[i], ElementType::ALTAR);
				}
			}
		}
		if (jUnique.count("unique_events")) {
			auto& jEvents = jUnique["unique_events"];
			if (jEvents.is_array()) {
				for (size_t i = 0; i < jEvents.size(); ++i) {
					parseChunk(jEvents[i], ElementType::EVENT);
				}
			}
		}
		if (jUnique.count("unique_enemies")) {
			json jEvents = jUnique["unique_enemies"];
			if (jEvents.is_array()) {
				for (size_t i = 0; i < jEvents.size(); ++i) {
					parseChunk(jEvents[i], ElementType::ENEMY);
				}
			}
		}
	}

	return true;
}

void CModuleUniques::parseChunk(const json & j, ElementType type) {
	assert(j.count("id"));
	std::string id = j.value("id", "");
	if (type == ENEMY) {
		parseEnemy(j, id);
	}
	else {
		UniqueElement element;
		element.done = j.value("done", false);
		element.position = loadVEC3(j["position"]);
		element.level = j.value("level", "");
		switch (type) {
			case ElementType::COIN:
				coins.emplace(id, element);
				break;
			case ElementType::CHRYSALIS:
				chrysalides.emplace(id, element);
				break;
			case ElementType::ALTAR:
				altars.emplace(id, element);
				break;
			case ElementType::EVENT:
				events.emplace(id, element);
				break;
		}
	}
}

void CModuleUniques::parseEnemy(const json & j, std::string id) {
	UniqueEnemy element;
	element.done = j.value("done", false);
	element.position = loadVEC3(j["position"]);
	element.level = j.value("level", "");
	std::string type = j.value("type", "");
	if (type == "dreidel") {
		element.type = DREIDEL;
	}
	else if (type == "kippah") {
		element.type = KIPPAH;
	}
	else {
		element.type = DREIDEL;
		dbg("Se ha puesto dreidel como enemigo por defecto en uniques\n");
	}
	enemies.emplace(id, element);
}

bool CModuleUniques::stop() {
	//clear maps
	coins.clear();
	chrysalides.clear();
	altars.clear();
	events.clear();
	enemies.clear();
	return true;
}

void CModuleUniques::update(float delta) {

}

UniqueElement* CModuleUniques::getUniqueCoin(std::string id) {
	if (coins.find(id) != coins.end()) {
		return &coins[id];
	}
	return nullptr;
}

UniqueElement* CModuleUniques::getUniqueChrysalis(std::string id) {
	if (chrysalides.find(id) != chrysalides.end()) {
		return &chrysalides[id];
	}
	return nullptr;
}

UniqueElement* CModuleUniques::getUniqueAltar(std::string id) {
	if (altars.find(id) != altars.end()) {
		return &altars[id];
	}
	return nullptr;
}

UniqueElement* CModuleUniques::getUniqueEvent(std::string id) {
	if (events.find(id) != events.end()) {
		return &events[id];
	}
	return nullptr;
}

UniqueEnemy * CModuleUniques::getUniqueEnemy(std::string id) {
	if (enemies.find(id) != enemies.end()) {
		return &enemies[id];
	}
	return nullptr;
}

bool CModuleUniques::setCoinTaken(std::string id, bool isTaken) {
	if (coins.find(id) != coins.end()) {
		coins[id].done = isTaken;
		return true;
	}
	else {
		dbg("No encuentro este unique coin!!\n");
		return false;
	}
}

bool CModuleUniques::setChrysalisTaken(std::string id, bool isTaken) {
	if (chrysalides.find(id) != chrysalides.end()) {
		chrysalides[id].done = isTaken;
		return true;
	}
	else {
		dbg("No encuentro este unique chrysalis!!\n");
		return false;
	}
}

bool CModuleUniques::setAltarBroken(std::string id, bool isBroken) {
	if (altars.find(id) != altars.end()) {
		altars[id].done = isBroken;
		return true;
	}
	else {
		dbg("No encuentro este unique altar!!\n");
		return false;
	}
}

bool CModuleUniques::setEventTriggered(std::string id, bool isTriggered) {
	if (events.find(id) != events.end()) {
		events[id].done = isTriggered;
		return true;
	}
	else {
		dbg("No encuentro este unique event!!\n");
		return false;
	}
}

bool CModuleUniques::setEnemyDead(std::string id, bool isDead) {
	if (enemies.find(id) != enemies.end()) {
		enemies[id].done = isDead;
		return true;
	}
	else {
		dbg("No encuentro este unique enemy!!\n");
		return false;
	}
}
