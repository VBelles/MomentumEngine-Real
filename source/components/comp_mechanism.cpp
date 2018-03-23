#include "mcv_platform.h"
#include "comp_mechanism.h"
#include "components/comp_transform.h"

DECL_OBJ_MANAGER("mechanism", TCompMechanism);

void TCompMechanism::debugInMenu() {
}

void TCompMechanism::registerMsgs() {
    DECL_MSG(TCompMechanism, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompMechanism, TMsgAttackHit, onHit);
}

void TCompMechanism::load(const json& j, TEntityParseContext& ctx) {
    desactivationTime = j.value("desactivationTime", 0.f);
	if(j.count("systems")) {
		auto& systems = j["systems"];
		assert(systems.is_array());
		for (auto& system : systems) {
			if (system.is_string()) {
				mechanismSystems.push_back(getEntityByName(system));
			}
		}
	}
}

void TCompMechanism::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	//mechanismSystems.push_back(mechanismSystemsNames....)
}

void TCompMechanism::update(float dt) {

}

void TCompMechanism::onHit(const TMsgAttackHit & msg) {

}
