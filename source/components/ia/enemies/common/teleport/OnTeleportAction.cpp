#include "mcv_platform.h"
#include "OnTeleportAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

OnTeleportAction::OnTeleportAction(Enemy* enemy, std::string animation):
	enemy(enemy),
	animation(animation){
}

int OnTeleportAction::execAction(float delta) {
	enemy->getSkeleton()->executeAction(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();
	return Leave;
}
