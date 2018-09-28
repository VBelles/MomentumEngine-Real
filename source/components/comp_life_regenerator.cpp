#include "mcv_platform.h"
#include "comp_life_regenerator.h"
#include "entity/common_msgs.h"
#include "components/player/attack_info.h"

DECL_OBJ_MANAGER("life_regenerator", TCompLifeRegenerator);

void TCompLifeRegenerator::registerMsgs() {
	DECL_MSG(TCompLifeRegenerator, TMsgTriggerEnter, onTriggerEnter);
}

void TCompLifeRegenerator::load(const json& j, TEntityParseContext& ctx) {
	hpRestored = j.value("HP restored", hpRestored);
}

void TCompLifeRegenerator::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	player = (CHandle)getEntityByName(PLAYER_NAME);
}
void TCompLifeRegenerator::onTriggerEnter(const TMsgTriggerEnter& msg) {
	CEntity* entity = msg.h_other_entity;
	/*AttackInfo atackInfo;
	atackInfo.damage = hpRestored;
	entity->sendMsg(TMsgAttackHit{ CHandle(this), atackInfo });*/
}
