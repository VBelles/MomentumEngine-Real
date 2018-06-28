#include "mcv_platform.h"
#include "OnBlockingBreakAction.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

OnBlockingBreakAction::OnBlockingBreakAction(Enemy* enemy, std::string animation): 
	enemy(enemy),
	animation(animation){
}

int OnBlockingBreakAction::execAction(float delta) {
	enemy->getSkeleton()->executeAction(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();
	enemy->blockingBroken = false;
	enemy->isBlocking = false;
	return Leave;
}
