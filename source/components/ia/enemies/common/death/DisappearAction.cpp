#include "mcv_platform.h"
#include "DisappearAction.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"
#include "components/player/comp_player_model.h"

REGISTER_BTACTION("DisappearAction", DisappearAction);

DisappearAction::DisappearAction() {
	type = "DisappearAction";
}

DisappearAction::DisappearAction(Enemy* enemy, std::string animation) :
	DisappearAction() {
	this->enemy = enemy;
	this->animation = animation;
}

int DisappearAction::execAction(float delta) {
	if (!enemy->getCollider()->toDestroy &&
		enemy->animationTimer.elapsed() > enemy->getSkeleton()->getAnimationDuration(animation)) {
		if (!enemy->attackTarget.empty()) {
			CEntity* entity = enemy->getEntityHandle();
			enemy->getPlayerModel()->removeAttacker(entity->getName(), enemy->attackSlots);
			enemy->attackTarget = "";
		}
		enemy->getCollider()->toDestroy = true; //TODO enable/disable bien
		enemy->getCollider()->destroy();
		return Leave;
	}
	return Stay;
}

void DisappearAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}

void DisappearAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
}