#include "mcv_platform.h"
#include "comp_mechanism_system.h"
#include "components/comp_transform.h"
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
	//activationEffectFile = j["activationEffectFile"].get<std::string>();
}

void TCompMechanismSystem::onAllScenesCreated(const TMsgAllScenesCreated & msg) {
	for (auto& mechanismName : mechanismsNames) {
		mechanisms.push_back(getEntityByName(mechanismName));
	}
}

void TCompMechanismSystem::update(float dt) {
	//Comprobar que todos están activados
	//if (!isActivated) {
	//	if (numberOfMechanismsActivated == numberOfMechanisms) {
	//		EngineScripting.doFile(activationEffectFile);
	//		isActivated = true;
	//	}
	//}
}

void TCompMechanismSystem::onActivate(const TMsgMechanismActivated & msg) {
	numberOfMechanismsActivated++;
	if (numberOfMechanismsActivated > numberOfMechanisms) {
		numberOfMechanismsActivated = numberOfMechanisms;
		dbg("Number of mechanisms activated more than max!! Something happens...\n");
	}
	else if (numberOfMechanismsActivated == numberOfMechanisms) {
        dbg("mechanism system activated\n");
		Engine.getScripting().throwEvent(onMechanismSystemActivated, ((CEntity*)CHandle(this).getOwner())->getName());
	}
}

void TCompMechanismSystem::onDeactivate(const TMsgMechanismDeactivated & msg) {
	numberOfMechanismsActivated--;
	if (numberOfMechanismsActivated == (numberOfMechanisms - 1)) {
		Engine.getScripting().throwEvent(onMechanismSystemDeactivated, ((CEntity*)CHandle(this).getOwner())->getName());
	}
	else if (numberOfMechanismsActivated < 0) {
		numberOfMechanismsActivated = 0;
		dbg("Number of mechanisms activated fewer than 0!! Something happens...\n");
	}
}