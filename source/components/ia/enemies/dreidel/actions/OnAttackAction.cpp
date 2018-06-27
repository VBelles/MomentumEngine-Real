#include "mcv_platform.h"
#include "OnAttackAction.h"

#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"

OnAttackAction::OnAttackAction(Enemy* enemy): enemy(enemy) {
}

int OnAttackAction::execAction(float delta) {
	enemy->updateGravity(delta);
	enemy->rotateTowards(delta, enemy->getPlayerTransform()->getPosition(), enemy->rotationSpeed);
	if (enemy->attackTimer.elapsed() < enemy->attackCooldown) {
		enemy->setCurrent(nullptr);
	}
	else {
		enemy->hasAttacked = false;
		enemy->attackTimer.reset();
		enemy->getSkeleton()->executeAction(2, 0.2f, 0.2f);
	}
	return Leave;
}
