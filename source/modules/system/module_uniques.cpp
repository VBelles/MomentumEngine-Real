#include "mcv_platform.h"
#include "module_uniques.h"
#include "modules/module.h"
#include "entity/entity.h"
#include "modules/system/module_entities.h"
#include "handle/handle.h"
#include "components/comp_name.h"
#include "components/comp_transform.h"
#include "entity/entity_parser.h"

bool CModuleUniques::start() {
	//de momento json, pero debería estar encriptado
	//json jcoins = loadJson("data/unique_coins.json");
	//json jchrysalis = loadJson("data/unique_chrysalis.json");
	//json jaltars = loadJson("data/unique_altars.json");
	//json jevents = loadJson("data/unique_events.json");

	//parse


	return true;
}

bool CModuleUniques::stop() {
	//clear maps
	coins.clear();
	chrysalis.clear();
	altars.clear();
	events.clear();
	return true;
}

void CModuleUniques::update(float delta) {

}

UniqueThing* CModuleUniques::getUniqueCoin(std::string id) {
	if (coins.find(id) != coins.end()) {
		return &coins[id];
	}
	return nullptr;
}

UniqueThing* CModuleUniques::getUniqueChrysalis(std::string id) {
	if (chrysalis.find(id) != chrysalis.end()) {
		return &chrysalis[id];
	}
	return nullptr;
}

UniqueThing* CModuleUniques::getUniqueAltar(std::string id) {
	if (altars.find(id) != altars.end()) {
		return &altars[id];
	}
	return nullptr;
}

UniqueThing* CModuleUniques::getUniqueEvent(std::string id) {
	if (events.find(id) != events.end()) {
		return &events[id];
	}
	return nullptr;
}

void CModuleUniques::setCoinTaken(std::string id, bool isTaken) {
	if (coins.find(id) != coins.end()) {
		coins[id].done = isTaken;
	}
	else {
		dbg("No encuentro este unique!!\n");
	}
}

void CModuleUniques::setChrysalisTaken(std::string id, bool isTaken) {
	if (chrysalis.find(id) != chrysalis.end()) {
		chrysalis[id].done = isTaken;
	}
	else {
		dbg("No encuentro este unique!!\n");
	}
}

void CModuleUniques::setAltarBroken(std::string id, bool isBroken) {
	if (altars.find(id) != altars.end()) {
		altars[id].done = isBroken;
	}
	else {
		dbg("No encuentro este unique!!\n");
	}
}

void CModuleUniques::setEventTriggered(std::string id, bool isTriggered) {
	if (events.find(id) != events.end()) {
		events[id].done = isTriggered;
	}
	else {
		dbg("No encuentro este unique!!\n");
	}
}
