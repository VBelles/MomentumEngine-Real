#include "mcv_platform.h"
#include "OnAppearAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

OnAppearAction::OnAppearAction(Enemy* enemy, std::string animation):
	enemy(enemy),
	animation(animation){
}

int OnAppearAction::execAction(float delta) {
	enemy->getSkeleton()->executeAction(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();
	return Leave;
}
