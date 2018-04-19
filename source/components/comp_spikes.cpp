#include "mcv_platform.h"
#include "comp_spikes.h"
#include "entity/common_msgs.h"
#include "components/comp_collider.h"

DECL_OBJ_MANAGER("spikes", TCompSpikes);

void TCompSpikes::registerMsgs() {
	DECL_MSG(TCompSpikes, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompSpikes, TMsgTriggerEnter, onTriggerEnter);
}

void TCompSpikes::load(const json& j, TEntityParseContext& ctx) {
	damage = j.value("damage", 0.f);
}

void TCompSpikes::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
}

void TCompSpikes::onTriggerEnter(const TMsgTriggerEnter& msg) {
	CEntity* entity = msg.h_other_entity;
	AttackInfo atackInfo;
	atackInfo.damage = 2.f;
	entity->sendMsg(TMsgAttackHit{ CHandle(this), atackInfo });
}

void TCompSpikes::update(float delta) {

}