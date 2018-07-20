#include "mcv_platform.h"
#include "OnIdleWarAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

REGISTER_BTACTION("OnIdleWarAction", OnIdleWarAction);

OnIdleWarAction::OnIdleWarAction() {
	type = "OnIdleWarAction";
}

OnIdleWarAction::OnIdleWarAction(Enemy* enemy, std::string animation) :
	OnIdleWarAction() {
	this->enemy = enemy;
	this->animation = animation;
}

int OnIdleWarAction::execAction(float delta) {
	VEC3 playerPosition = enemy->getPlayerTransform()->getPosition();
	enemy->rotateTowards(delta, playerPosition, enemy->rotationSpeed);
	enemy->getSkeleton()->blendCycle(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();
	return Leave;
}

void OnIdleWarAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}
