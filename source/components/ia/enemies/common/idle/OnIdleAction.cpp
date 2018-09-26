#include "mcv_platform.h"
#include "OnIdleAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"
#include "components/player/comp_player_model.h"

REGISTER_BTACTION("OnIdleAction", OnIdleAction);

OnIdleAction::OnIdleAction() {
	type = "OnIdleAction";
}

OnIdleAction::OnIdleAction(Enemy* enemy, std::string animation) :
	OnIdleAction() {
	this->enemy = enemy;
	this->animation = animation;
}

int OnIdleAction::execAction(float delta) {
	if (!enemy->attackTarget.empty()) {
		CEntity* entity = enemy->getEntityHandle();
		enemy->getPlayerModel()->removeAttacker(entity->getName(), enemy->attackSlots);
		enemy->attackTarget = "";
	}

	enemy->getSkeleton()->executeAction(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();
	return Leave;
}

void OnIdleAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}

void OnIdleAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
}