#include "mcv_platform.h"
#include "DisappearAction.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

DisappearAction::DisappearAction(Enemy* enemy, std::string animation) :
	enemy(enemy),
	animation(animation) {
}

int DisappearAction::execAction(float delta) {
	if (enemy->animationTimer.elapsed() < enemy->getSkeleton()->getAnimationDuration(animation)) {
		return Stay;
	}
	else {
		enemy->getCollider()->toDestroy = true; //TODO enable/disable bien
		enemy->getCollider()->destroy();
		return Leave;
	}
}
