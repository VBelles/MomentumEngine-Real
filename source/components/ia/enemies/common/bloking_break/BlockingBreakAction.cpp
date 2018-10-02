#include "mcv_platform.h"
#include "BlockingBreakAction.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("BlockingBreakAction", BlockingBreakAction);

BlockingBreakAction::BlockingBreakAction() {
	type = "BlockingBreakAction";
}

BlockingBreakAction::BlockingBreakAction(Enemy* enemy, std::string animation) :
	BlockingBreakAction() {
	this->enemy = enemy;
	this->animation = animation;
}

int BlockingBreakAction::execAction(float delta) {
	if (enemy->animationTimer.elapsed() < enemy->getSkeleton()->getAnimationDuration(animation)) {
		return Stay;
	}
	return Leave;
}

void BlockingBreakAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}

void BlockingBreakAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
}
