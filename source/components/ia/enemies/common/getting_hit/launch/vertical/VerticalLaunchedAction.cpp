#include "mcv_platform.h"
#include "VerticalLaunchedAction.h"
#include "components/comp_collider.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_hitboxes.h"

VerticalLaunchedAction::VerticalLaunchedAction(Enemy* enemy) : enemy(enemy) {
}

int VerticalLaunchedAction::execAction(float delta) {
	enemy->updateGravity(delta);
	if (enemy->velocity.y <= 0) {
		enemy->timer.reset();
		enemy->getHitboxes()->disable(enemy->attacks[attack].hitboxName);
		enemy->currentAttack = "";
		return Leave;
	}
	else {
		return Stay;
	}
}
