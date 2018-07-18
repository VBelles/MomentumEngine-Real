#include "mcv_platform.h"
#include "OnBlockingBreakAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_give_power.h"

REGISTER_BTAction("OnBlockingBreakAction", OnBlockingBreakAction);

OnBlockingBreakAction::OnBlockingBreakAction(Enemy* enemy, std::string animation): 
	enemy(enemy),
	animation(animation){
}

int OnBlockingBreakAction::execAction(float delta) {
	enemy->getSkeleton()->executeAction(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();
	enemy->blockingBroken = false;
	enemy->isBlocking = false;
	enemy->getPower()->setStateMultiplier(1.f);
	return Leave;
}

void OnBlockingBreakAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}