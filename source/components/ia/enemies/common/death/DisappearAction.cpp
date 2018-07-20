#include "mcv_platform.h"
#include "DisappearAction.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("DisappearAction", DisappearAction);

DisappearAction::DisappearAction() {
	type = "DisappearAction";
}

DisappearAction::DisappearAction(Enemy* enemy, std::string animation) :
	DisappearAction() {
	this->enemy = enemy;
	this->animation = animation;
}

int DisappearAction::execAction(float delta) {
	if (!enemy->getCollider()->toDestroy &&
		enemy->animationTimer.elapsed() > enemy->getSkeleton()->getAnimationDuration(animation)) {
		enemy->getCollider()->toDestroy = true; //TODO enable/disable bien
		enemy->getCollider()->destroy();
	}
	return Stay;
}

void DisappearAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}
