#include "mcv_platform.h"
#include "OnIdleLoop.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

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
	enemy->getSkeleton()->blendCycle(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();
	return Leave;
}

void OnIdleLoop::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}