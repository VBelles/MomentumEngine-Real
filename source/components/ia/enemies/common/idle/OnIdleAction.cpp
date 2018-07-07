#include "mcv_platform.h"
#include "OnIdleAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

OnIdleAction::OnIdleAction(Enemy* enemy, std::string animation):
	enemy(enemy),
	animation(animation){
}

int OnIdleAction::execAction(float delta) {
	enemy->getSkeleton()->executeAction(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();
	return Leave;
}
