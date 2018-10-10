#include "mcv_platform.h"
#include "OnIdleLoop.h"
#include "components/ia/enemies/Enemy.h"
#include "components/player/comp_player_model.h"

REGISTER_BTACTION("OnIdleLoop", OnIdleLoop);

OnIdleLoop::OnIdleLoop() {
	type = "OnIdleLoop";
}

OnIdleLoop::OnIdleLoop(Enemy* enemy, std::string animation) :
	OnIdleLoop() {
	this->enemy = enemy;
	this->animation = animation;
}

int OnIdleLoop::execAction(float delta) {
	if (!enemy->attackTarget.empty()) {
		CEntity* entity = enemy->getEntityHandle();
		enemy->getPlayerModel()->removeAttacker(entity->getName(), enemy->attackSlots);
		enemy->attackTarget = "";
	}

	enemy->getSkeleton()->blendCycle(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();
	enemy->getSound()->play("idle");
	return Leave;
}

void OnIdleLoop::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}

void OnIdleLoop::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
}
