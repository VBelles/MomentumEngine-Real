#include "mcv_platform.h"
#include "IdleWarActionFlying.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_transform.h"

IdleWarActionFlying::IdleWarActionFlying(Enemy* enemy, std::string animation) :
	enemy(enemy),
	animation(animation) {
}

int IdleWarActionFlying::execAction(float delta) {
	VEC3 playerPosition = enemy->getPlayerTransform()->getPosition();
	enemy->rotateTowards(delta, playerPosition, enemy->rotationSpeed);
	if (enemy->animationTimer.elapsed() < enemy->getSkeleton()->getAnimationDuration(animation)) {
		return Stay;
	}
	return Leave;
}
