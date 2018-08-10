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
	//no es necesario que cada tipo esté separado en un archivo diferente
	auto files = WindowsUtils::getAllFiles("data/uniques/", "*.json");
	std::vector<json> jUniques;
	for (auto& file : files) {
		jUniques.push_back(loadJson(file));
	}

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
		if (jUnique.count("unique_life_pieces")) {
			auto& jLifePieces = jUnique["unique_life_pieces"];
			if (jLifePieces.is_array()) {
				for (size_t i = 0; i < jLifePieces.size(); ++i) {
					parseChunk(jLifePieces[i], ElementType::LIFEPIECE);
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
		if (jUnique.count("unique_power_up")) {
			json jPowerUps = jUnique["unique_power_up"];
			if (jPowerUps.is_array()) {
				for (size_t i = 0; i < jPowerUps.size(); ++i) {
					parseChunk(jPowerUps[i], ElementType::POWERUP);
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
	else if (type == POWERUP) {
		parsePowerUp(j, id);
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

void CModuleUniques::parsePowerUp(const json & j, std::string id) {
	UniquePowerUp element;
	element.done = j.value("done", false);
	element.position = loadVEC3(j["position"]);
	element.level = j.value("level", "");
	element.stateToUnlock = j.value("state", "");
	powerUps.emplace(id, element);
}

bool CModuleUniques::stop() {
	//clear maps
	coins.clear();
	chrysalides.clear();
	altars.clear();
	events.clear();
	lifePieces.clear();
	enemies.clear();
	powerUps.clear();
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

UniqueElement * CModuleUniques::getUniqueLifePiece(std::string id) {
	if (lifePieces.find(id) != lifePieces.end()) {
		return &lifePieces[id];
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

UniquePowerUp * CModuleUniques::getUniquePowerUp(std::string id) {
	if (powerUps.find(id) != powerUps.end()) {
		return &powerUps[id];
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

bool CModuleUniques::setLifePieceTaken(std::string id, bool isTaken) {
	if (lifePieces.find(id) != lifePieces.end()) {
		lifePieces[id].done = isTaken;
		return true;
	}
	else {
		dbg("No encuentro esta unique life piece!!\n");
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

bool CModuleUniques::setPowerUpTaken(std::string id, bool isTaken) {
	if (powerUps.find(id) != powerUps.end()) {
		powerUps[id].done = isTaken;
		return true;
	}
	else {
		dbg("No encuentro este unique power-up!!\n");
		return false;
	}
}
