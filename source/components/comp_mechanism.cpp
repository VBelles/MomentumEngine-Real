#include "mcv_platform.h"
#include "comp_mechanism.h"
#include "components/comp_transform.h"
#include "entity/common_msgs.h"
#include "components/comp_collider.h"
#include "components/player/attack_info.h"

DECL_OBJ_MANAGER("mechanism", TCompMechanism);

void TCompMechanism::debugInMenu() {
}

void TCompMechanism::registerMsgs() {
    DECL_MSG(TCompMechanism, TMsgAllScenesCreated, onAllScenesCreated);
	DECL_MSG(TCompMechanism, TMsgAttackHit, onHit);
}

void TCompMechanism::load(const json& j, TEntityParseContext& ctx) {
    deactivationTime = j.value("deactivationTime", 0.f);
	isResettable = j.value("isResettable", true);
	reactivationTime = j.value("reactivationTime", 0.f);
	mechanismSystemsNames.clear();
	if(j.count("systems")) {
		auto& systems = j["systems"];
		assert(systems.is_array());
		for (auto& system : systems) {
			if (system.is_string()) {
				mechanismSystemsNames.push_back(system);
			}
		}
	}
	isActivated = false;
}

void TCompMechanism::onAllScenesCreated(const TMsgAllScenesCreated & msg) {
	mechanismSystems.clear();
	for (auto& systemName : mechanismSystemsNames) {
		mechanismSystems.push_back(getEntityByName(systemName));
	}
	reactivationTimer.reset();
}

void TCompMechanism::update(float dt) {
	if (isActivated && isResettable && deactivationTimer.elapsed() >= deactivationTime) {
		isActivated = false;
		reactivationTimer.reset();
		for (auto& system : mechanismSystems) {
			CEntity* entity = system;
			dbg("Sending deactivating msg.\n");
			entity->sendMsg(TMsgMechanismDeactivated{});
		}
	}
}

void TCompMechanism::onHit(const TMsgAttackHit & msg) {
	if (msg.info.activatesMechanism) {
        dbg("Mechanism activated\n");

		if (!isActivated) {
			if (reactivationTimer.elapsed() >= reactivationTime) {
				deactivationTimer.reset();
				//activate
				isActivated = true;
				for (auto& system : mechanismSystems) {
					CEntity* entity = system;
					dbg("Sending activating msg %s.\n", entity->getName());
					entity->sendMsg(TMsgMechanismActivated{});
				}
			}
		}
		else if (isResettable) {
			deactivationTimer.reset();
		}
	}
}
