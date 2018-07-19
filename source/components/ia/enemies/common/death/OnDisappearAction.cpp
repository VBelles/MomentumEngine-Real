#include "mcv_platform.h"
#include "OnDisappearAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("OnDisappearAction", OnDisappearAction);

OnDisappearAction::OnDisappearAction(Enemy* enemy, std::string animation):
	enemy(enemy),
	animation(animation){
}

int OnDisappearAction::execAction(float delta) {
	enemy->getSkeleton()->executeAction(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();
	return Leave;
}

void OnDisappearAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}