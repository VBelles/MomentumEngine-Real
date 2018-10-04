#include "mcv_platform.h"
#include "comp_spikes.h"
#include "entity/common_msgs.h"
#include "components/player/attack_info.h"

DECL_OBJ_MANAGER("spikes", TCompSpikes);

void TCompSpikes::registerMsgs() {
	DECL_MSG(TCompSpikes, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompSpikes, TMsgTriggerExit, onTriggerExit);
}

void TCompSpikes::update(float delta) {
	if (timer.elapsed() >= hitAgainTime) {
		timer.reset();
		AttackInfo atackInfo;
		atackInfo.damage = damage;
		atackInfo.invulnerabilityTime = invulnerabilityTime;
		atackInfo.stun = new AttackInfo::Stun{ stunTime };

		std::set<CHandle> handlesToErase;
		for (auto& handle : handles) {
			if (handle.isValid()) {
				CEntity* entity = handle;
				entity->sendMsg(TMsgAttackHit{ CHandle(this).getOwner(), atackInfo });
			}
			else {
				handlesToErase.insert(handle);
			}
		}
		
		for (auto& handle : handlesToErase) {
			handles.erase(handle);
		}
	}
}

void TCompSpikes::load(const json& j, TEntityParseContext& ctx) {
	damage = j.value("damage", 0.f);
	invulnerabilityTime = j.value("invulnerability_time", 0.f);
	hitAgainTime = j.value("hit_again_time", 0.f);
	stunTime = j.value("stun_time", 0.f);
}

void TCompSpikes::onTriggerEnter(const TMsgTriggerEnter& msg) {
	CEntity* entity = msg.h_other_entity;
	AttackInfo atackInfo;
	atackInfo.damage = damage;
	atackInfo.invulnerabilityTime = invulnerabilityTime;
	atackInfo.stun = new AttackInfo::Stun{ stunTime };
	entity->sendMsg(TMsgAttackHit{ CHandle(this).getOwner(), atackInfo });
	handles.insert(msg.h_other_entity);
}

void TCompSpikes::onTriggerExit(const TMsgTriggerExit & msg) {
	handles.erase(msg.h_other_entity);
}
