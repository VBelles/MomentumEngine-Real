#include "mcv_platform.h"
#include "TeleportAction.h"
#include "components/ia/enemies/Enemy.h"

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
	if (enemy->animationTimer.elapsed() < enemy->getSkeleton()->getAnimationDuration(animation) - animationEndPadding) {
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
	animationEndPadding = j.value("enimation_end_padding", animationEndPadding);
}

void TeleportAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
	ImGui::DragFloat("Animation end padding", &animationEndPadding, 0.01f, 0.f, 50.f);
}
