#include "mcv_platform.h"
#include "StepBackAction.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_transform.h"

StepBackAction::StepBackAction(Enemy* enemy, std::string animation, float speed) :
	enemy(enemy),
	animation(animation),
	speed(speed) {
}

int StepBackAction::execAction(float delta) {
	enemy->updateGravity(delta);
	if (enemy->animationTimer.elapsed() < enemy->getSkeleton()->getAnimationDuration(animation)) {
		VEC3 stepBackMovement = -enemy->getTransform()->getFront() * speed * delta;
		VEC3 pos = enemy->getTransform()->getPosition();
		if (enemy->navmesh->existsConnection(pos, pos + stepBackMovement)) {
			enemy->deltaMovement += stepBackMovement;
			return Stay;
		}
	}
	return Leave;
}
