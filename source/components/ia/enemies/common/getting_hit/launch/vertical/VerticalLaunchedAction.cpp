#include "mcv_platform.h"
#include "VerticalLaunchedAction.h"
#include "components/comp_collider.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"

VerticalLaunchedAction::VerticalLaunchedAction(Enemy* enemy) : enemy(enemy) {
}

int VerticalLaunchedAction::execAction(float delta) {
	enemy->updateGravity(delta);
	if (enemy->velocity.y <= 0) {
		enemy->timer.reset();
		return Leave;
	}
	else {
		return Stay;
	}
}
