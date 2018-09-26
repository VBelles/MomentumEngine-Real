#include "mcv_platform.h"
#include "HitStun.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("HitStun", HitStun);

HitStun::HitStun() {
	type = "HitStun";
}

HitStun::HitStun(Enemy* enemy, std::string animation) :
	HitStun() {
	this->enemy = enemy;
	this->animation = animation;
}

int HitStun::execAction(float delta) {
	if (enemy->animationTimer.elapsed() < enemy->getSkeleton()->getAnimationDuration(animation)) {
		return Stay;
	}
	return Leave;
}

void HitStun::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}

void HitStun::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
}