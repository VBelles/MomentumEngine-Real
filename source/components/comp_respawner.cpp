#include "mcv_platform.h"
#include "comp_respawner.h"
#include "handle/handle.h"

DECL_OBJ_MANAGER("respawner", TCompRespawner);

void TCompRespawner::debugInMenu() {
}

void TCompRespawner::load(const json& j, TEntityParseContext& ctx) {
	respawnTime = j.value("respawnTime", 5.f);
	timer.reset();
}

void TCompRespawner::update(float delta) {
	if (timer.elapsed() >= respawnTime) {
		CEntity* entity = CHandle(this).getOwner();
		entity->sendMsg(TMsgRespawn{});
		timer.reset();
	}
}