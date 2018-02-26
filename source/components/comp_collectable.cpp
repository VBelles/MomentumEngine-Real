#include "mcv_platform.h"
#include "comp_collectable.h"
#include "components/comp_transform.h"

DECL_OBJ_MANAGER("collectable", TCompCollectable);

void TCompCollectable::debugInMenu() {
}

void TCompCollectable::registerMsgs() {
    DECL_MSG(TCompCollectable, TMsgEntitiesGroupCreated, onGroupCreated);
    DECL_MSG(TCompCollectable, TMsgTriggerEnter, onCollect);
}

void TCompCollectable::load(const json& j, TEntityParseContext& ctx) {
}

void TCompCollectable::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
}

void TCompCollectable::update(float dt) {
	if (collected) {
		CEntity *player = (CEntity*)getEntityByName("The Player");
		TMsgCollect msg{ "chrysalis" };
		player->sendMsg(msg);

		TCompCollider *collider = get<TCompCollider>();
		collider->actor->release();
		CHandle(this).getOwner().destroy();
	}
}

void TCompCollectable::onCollect(const TMsgTriggerEnter & msg) {
	if (!collected) {
		CEntity *collector = msg.h_other_entity;
		std::string collectorName = collector->getName();
		dbg("Trigger enter by %s\n", collectorName.c_str());
		if (collectorName.compare("The Player") == 0) {
			collected = true;
		}
	}
}
