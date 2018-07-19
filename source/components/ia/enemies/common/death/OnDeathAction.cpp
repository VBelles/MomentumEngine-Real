#include "mcv_platform.h"
#include "OnDeathAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("OnDeathAction", OnDeathAction);

OnDeathAction::OnDeathAction(Enemy* enemy, std::string animation):
	enemy(enemy),
	animation(animation){
}

int OnDeathAction::execAction(float delta) {
	enemy->isBlocking = false;
	enemy->blockingBroken = false;
	enemy->getSkeleton()->executeAction(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();
	return Leave;
}

void OnDeathAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}