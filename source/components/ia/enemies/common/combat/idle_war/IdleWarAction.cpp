#include "mcv_platform.h"
#include "IdleWarAction.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("IdleWarAction", IdleWarAction);

IdleWarAction::IdleWarAction() {
	type = "IdleWarAction";
}

IdleWarAction::IdleWarAction(Enemy* enemy, std::string animation) :
	IdleWarAction() {
	this->enemy = enemy;
	this->animation = animation;
}

int IdleWarAction::execAction(float delta) {
	enemy->updateGravity(delta);
	VEC3 playerPosition = enemy->getPlayerTransform()->getPosition();
	enemy->rotateTowards(delta, playerPosition, enemy->rotationSpeed);
	if (enemy->animationTimer.elapsed() < enemy->getSkeleton()->getAnimationDuration(animation)) {
		return Stay;
	}
	return Leave;
}

void IdleWarAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}

void IdleWarAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
}
