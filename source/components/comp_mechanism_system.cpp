#include "mcv_platform.h"
#include "comp_mechanism_system.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("mechanism_system", TCompMechanismSystem);

void TCompMechanismSystem::debugInMenu() {
}

void TCompMechanismSystem::registerMsgs() {
    DECL_MSG(TCompMechanismSystem, TMsgAllScenesCreated, onAllScenesCreated);
	DECL_MSG(TCompMechanismSystem, TMsgMechanismActivated, onActivate);
	DECL_MSG(TCompMechanismSystem, TMsgMechanismDeactivated, onDeactivate);
}

void TCompMechanismSystem::load(const json& j, TEntityParseContext& ctx) {
	numberOfMechanisms = j.value("number_of_mechanisms", 0);
	numberOfMechanismsActivated = 0;
}

void TCompMechanismSystem::onAllScenesCreated(const TMsgAllScenesCreated & msg) {
	for (auto& mechanismName : mechanismsNames) {
		mechanisms.push_back(getEntityByName(mechanismName));
	}
}

void TCompMechanismSystem::update(float dt) {

}

void TCompMechanismSystem::onActivate(const TMsgMechanismActivated & msg) {
	numberOfMechanismsActivated++;
	if (numberOfMechanismsActivated > numberOfMechanisms) {
		numberOfMechanismsActivated = numberOfMechanisms;
		dbg("Number of mechanisms activated more than max!! Something happens...\n");
	}
	else if (numberOfMechanismsActivated == numberOfMechanisms) {
		dbg("number of mechanisms %i \n", numberOfMechanisms);
		EngineScripting.throwEvent(onMechanismSystemActivated, ((CEntity*)CHandle(this).getOwner())->getName());
		CEntity* entity = CHandle(this).getOwner();
		entity->sendMsg(TMsgMechanismSystemActivated{});
	}
}

void TCompMechanismSystem::onDeactivate(const TMsgMechanismDeactivated & msg) {
	numberOfMechanismsActivated--;
	if (numberOfMechanismsActivated == (numberOfMechanisms - 1)) {
		EngineScripting.throwEvent(onMechanismSystemDeactivated, ((CEntity*)CHandle(this).getOwner())->getName());
		CEntity* entity = CHandle(this).getOwner();
		entity->sendMsg(TMsgMechanismSystemDeactivated{});
	}
	else if (numberOfMechanismsActivated < 0) {
		numberOfMechanismsActivated = 0;
		dbg("Number of mechanisms activated fewer than 0!! Something happens...\n");
	}
}
