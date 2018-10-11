#include "mcv_platform.h"
#include "DisappearAction.h"
#include "components/ia/enemies/Enemy.h"

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
		enemy->animationTimer.elapsed() > enemy->getSkeleton()->getAnimationDuration(animation) - animationEndPadding) {
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
	animationEndPadding = j.value("enimation_end_padding", animationEndPadding);
}

void DisappearAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
	ImGui::DragFloat("Animation end padding", &animationEndPadding, 0.01f, 0.f, 50.f);
}
