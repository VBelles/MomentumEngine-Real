#include "mcv_platform.h"
#include "OnIdleWarAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

OnIdleWarAction::OnIdleWarAction(Enemy* enemy, std::string animation):
	enemy(enemy),
	animation(animation){
}

int OnIdleWarAction::execAction(float delta) {
	VEC3 playerPosition = enemy->getPlayerTransform()->getPosition();
	enemy->rotateTowards(delta, playerPosition, enemy->rotationSpeed);
	enemy->getSkeleton()->blendCycle(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();
	return Leave;
}
