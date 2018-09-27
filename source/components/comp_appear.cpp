#include "mcv_platform.h"
#include "comp_appear.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"


DECL_OBJ_MANAGER("appear", TCompAppear);

void TCompAppear::debugInMenu() {
}

void TCompAppear::registerMsgs() {
	DECL_MSG(TCompAppear, TMsgAllScenesCreated, onAllScenesCreated);
	DECL_MSG(TCompAppear, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompAppear, TMsgMechanismSystemActivated, onActivate);
}

void TCompAppear::load(const json& j, TEntityParseContext& ctx) {
}

void TCompAppear::onAllScenesCreated(const TMsgAllScenesCreated & msg) {
}

void TCompAppear::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	TCompRender *render = get<TCompRender>();
	render->disable();
	TCompCollider *collider = get<TCompCollider>();
	collider->destroy();
}

void TCompAppear::onActivate(const TMsgMechanismSystemActivated & msg) {
    TCompCollider *collider = get<TCompCollider>();
	collider->create();
	TCompRender *render = get<TCompRender>();
	render->enable();
}
