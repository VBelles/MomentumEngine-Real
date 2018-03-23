#include "mcv_platform.h"
#include "comp_mechanism_system.h"
#include "components/comp_transform.h"

DECL_OBJ_MANAGER("mechanism_system", TCompMechanismSystem);

void TCompMechanismSystem::debugInMenu() {
}

void TCompMechanismSystem::registerMsgs() {
    DECL_MSG(TCompMechanismSystem, TMsgAllScenesCreated, onAllScenesCreated);
	DECL_MSG(TCompMechanismSystem, TMsgMechanismActivated, onActivate);
	DECL_MSG(TCompMechanismSystem, TMsgMechanismDeactivated, onDeactivate);
}

void TCompMechanismSystem::load(const json& j, TEntityParseContext& ctx) {
	if(j.count("mechanisms")) {
		auto& mechanisms = j["mechanisms"];
		assert(mechanisms.is_array());
		for (auto& mechanism : mechanisms) {
			if (mechanism.is_string()) {
				mechanismsNames.push_back(mechanism);
			}
		}
		numberOfMechanisms = mechanisms.size();
	}
}

void TCompMechanismSystem::onAllScenesCreated(const TMsgAllScenesCreated & msg) {
	for (auto& mechanismName : mechanismsNames) {
		mechanisms.push_back(getEntityByName(mechanismName));
	}
}

void TCompMechanismSystem::update(float dt) {
	//Comprobar que todos están activados
	if (numberOfMechanismsActivated == numberOfMechanisms) {
		//Enviar mensaje a owner
		CHandle handle = this;
		CEntity* entity = handle.getOwner();
		entity->sendMsg(TMsgMechanismSystemActivated{});
	}
}

void TCompMechanismSystem::onActivate(const TMsgMechanismActivated & msg) {
	numberOfMechanismsActivated++;
	if (numberOfMechanismsActivated > numberOfMechanisms) {
		numberOfMechanismsActivated = numberOfMechanisms;
		dbg("Number of mechanisms activated more than max!! Something happens...\n");
	}
}

void TCompMechanismSystem::onDeactivate(const TMsgMechanismDeactivated & msg) {
	numberOfMechanismsActivated--;
	if (numberOfMechanismsActivated < 0) {
		numberOfMechanismsActivated = 0;
		dbg("Number of mechanisms activated fewer than 0!! Something happens...\n");
	}
}