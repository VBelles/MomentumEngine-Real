#include "mcv_platform.h"
#include "comp_respawner.h"
#include "handle/handle.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("respawner", TCompRespawner);

void TCompRespawner::load(const json& j, TEntityParseContext& ctx) {
	respawnTime = j.value("respawnTime", 5.f);
	timer.reset();
}

void TCompRespawner::update(float delta) {
	if (isDead && timer.elapsed() >= respawnTime) {
		CEntity* entity = CHandle(this).getOwner();
		entity->sendMsg(TMsgRespawn{});
		timer.reset();
		isDead = false;
	}
}

void TCompRespawner::onDead() {
	timer.reset();
	isDead = true;
}
