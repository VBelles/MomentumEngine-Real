#include "mcv_platform.h"
#include "IdleWarAction.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_transform.h"

IdleWarAction::IdleWarAction(Enemy* enemy, std::string animation) :
	enemy(enemy),
	animation(animation) {
}

int IdleWarAction::execAction(float delta) {
	enemy->updateGravity(delta);
	VEC3 playerPosition = enemy->getPlayerTransform()->getPosition();
	enemy->rotateTowards(delta, playerPosition, enemy->rotationSpeed);
	if (enemy->animationTimer.elapsed() < enemy->getSkeleton()->getAnimationDuration(animation)) {
		return Stay;
	}
	return Leave;
}
