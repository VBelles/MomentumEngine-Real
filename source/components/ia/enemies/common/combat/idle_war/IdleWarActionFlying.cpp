#include "mcv_platform.h"
#include "IdleWarActionFlying.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_transform.h"

REGISTER_BTACTION("IdleWarActionFlying", IdleWarActionFlying);

IdleWarActionFlying::IdleWarActionFlying() {
	type = "IdleWarActionFlying";
}

IdleWarActionFlying::IdleWarActionFlying(Enemy* enemy, std::string animation) :
	IdleWarActionFlying() {
	this->enemy = enemy;
	this->animation = animation;
}

int IdleWarActionFlying::execAction(float delta) {
	VEC3 playerPosition = enemy->getPlayerTransform()->getPosition();
	enemy->rotateTowards(delta, playerPosition, enemy->rotationSpeed);
	if (enemy->animationTimer.elapsed() < enemy->getSkeleton()->getAnimationDuration(animation)) {
		return Stay;
	}
	return Leave;
}

void IdleWarActionFlying::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}

void IdleWarActionFlying::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
}