#include "mcv_platform.h"
#include "TeleportAction.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_collider.h"

REGISTER_BTACTION("TeleportAction", TeleportAction);

TeleportAction::TeleportAction() {
	type = "TeleportAction";
}

TeleportAction::TeleportAction(Enemy* enemy, std::string animation) :
	TeleportAction() {
	this->enemy = enemy;
	this->animation = animation;
}

int TeleportAction::execAction(float delta) {
	if (enemy->animationTimer.elapsed() < enemy->getSkeleton()->getAnimationDuration(animation)) {
		return Stay;
	}
	else {
		enemy->getCollider()->controller->setFootPosition(PhysxUtils::toPxtendedVec3(enemy->spawnPosition));
		return Leave;
	}
}

void TeleportAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}

void TeleportAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
}