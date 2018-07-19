#include "mcv_platform.h"
#include "AppearAction.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"

REGISTER_BTACTION("AppearAction", AppearAction);

AppearAction::AppearAction(Enemy* enemy, std::string animation) :
	enemy(enemy),
	animation(animation) {
}

int AppearAction::execAction(float delta) {
	if (enemy->animationTimer.elapsed() < enemy->getSkeleton()->getAnimationDuration(animation)) {
		return Stay;
	}
	return Leave;
}

void AppearAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}