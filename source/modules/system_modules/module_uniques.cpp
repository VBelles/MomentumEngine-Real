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
	//de momento json, pero debería estar encriptado
	std::vector<json> jUniques;
	jUniques.push_back(loadJson("data/uniques/unique_coins.json"));//no hace falta que estén en archivos separados
	//jUniques.push_back(loadJson("data/unique_chrysalides.json"));
	//etc

	//parse
	for (json jUnique : jUniques) {
		if (jUnique.count("unique_coins")) {
			json jCoins = jUnique["unique_coins"];
			if (jCoins.is_array()) {
				for (size_t i = 0; i < jCoins.size(); ++i) {
					parseChunk(jCoins[i], ElementType::COIN);
				}
			}
		}
		if (jUnique.count("unique_chrysalides")) {
			json jChrysalides = jUnique["unique_chrysalides"];
			if (jChrysalides.is_array()) {
				for (size_t i = 0; i < jChrysalides.size(); ++i) {
					parseChunk(jChrysalides[i], ElementType::CHRYSALIS);
				}
			}
		}
		if (jUnique.count("unique_altars")) {
			json jAltars = jUnique["unique_altars"];
			if (jAltars.is_array()) {
				for (size_t i = 0; i < jAltars.size(); ++i) {
					parseChunk(jAltars[i], ElementType::ALTAR);
				}
			}
		}
		if (jUnique.count("unique_events")) {
			json jEvents = jUnique["unique_events"];
			if (jEvents.is_array()) {
				for (size_t i = 0; i < jEvents.size(); ++i) {
					parseChunk(jEvents[i], ElementType::EVENT);
				}
			}
		}
	}

	return true;
}

void CModuleUniques::parseChunk(const json & j, ElementType type) {
	assert(j.count("id"));
	std::string id = j.value("id", "");
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

bool CModuleUniques::stop() {
	//clear maps
	coins.clear();
	chrysalides.clear();
	altars.clear();
	events.clear();
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

bool CModuleUniques::setCoinTaken(std::string id, bool isTaken) {
	if (coins.find(id) != coins.end()) {
		coins[id].done = isTaken;
		return true;
	}
	else {
		dbg("No encuentro este unique!!\n");
		return false;
	}
}

bool CModuleUniques::setChrysalisTaken(std::string id, bool isTaken) {
	if (chrysalides.find(id) != chrysalides.end()) {
		chrysalides[id].done = isTaken;
		return true;
	}
	else {
		dbg("No encuentro este unique!!\n");
		return false;
	}
}

bool CModuleUniques::setAltarBroken(std::string id, bool isBroken) {
	if (altars.find(id) != altars.end()) {
		altars[id].done = isBroken;
		return true;
	}
	else {
		dbg("No encuentro este unique!!\n");
		return false;
	}
}

bool CModuleUniques::setEventTriggered(std::string id, bool isTriggered) {
	if (events.find(id) != events.end()) {
		events[id].done = isTriggered;
		return true;
	}
	else {
		dbg("No encuentro este unique!!\n");
		return false;
	}
}
