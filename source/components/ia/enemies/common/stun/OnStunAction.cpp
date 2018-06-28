#include "mcv_platform.h"
#include "OnStunAction.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

OnStunAction::OnStunAction(Enemy* enemy, std::string animation):
	enemy(enemy),
	animation(animation){
}

int OnStunAction::execAction(float delta) {
	enemy->getSkeleton()->blendCycle(animation, 0.1f, 0.1f);
	enemy->timer.reset();
	return Leave;
}
