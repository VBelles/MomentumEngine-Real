#include "mcv_platform.h"
#include "BlockingBreakAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTAction("BlockingBreakAction", BlockingBreakAction);

BlockingBreakAction::BlockingBreakAction(Enemy* enemy, std::string animation) :
	enemy(enemy),
	animation(animation) {
}

int BlockingBreakAction::execAction(float delta) {
	if (enemy->animationTimer.elapsed() < enemy->getSkeleton()->getAnimationDuration(animation)) {
		return Stay;
	}
	return Leave;
}

void BlockingBreakAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}