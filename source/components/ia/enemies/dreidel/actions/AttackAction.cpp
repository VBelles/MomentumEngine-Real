#include "mcv_platform.h"
#include "AttackAction.h"

#include "components/comp_transform.h"
#include "components/comp_hitboxes.h"
#include "skeleton/comp_skeleton.h"

AttackAction::AttackAction(Enemy* enemy): enemy(enemy) {
}

int AttackAction::execAction(float delta) {
	VEC2 attack = enemy->attacksFrameData["attack"];
	enemy->updateGravity(delta);
	enemy->rotateTowards(delta, enemy->getPlayerTransform()->getPosition(), enemy->rotationSpeed);
	if (enemy->attackTimer.elapsed() < (enemy->getSkeleton()->getAnimationDuration(2))) {
		if (!enemy->hasAttacked && enemy->attackTimer.elapsed() >= frames2sec(attack.x)) {
			enemy->getHitboxes()->enable("attack");
			enemy->hasAttacked = true;
		}
		else if (enemy->hasAttacked && enemy->attackTimer.elapsed() >= frames2sec(attack.x + attack.y)) {
			enemy->getHitboxes()->disable("attack");
		}
		return Stay;
	}
	else {
		enemy->getHitboxes()->disable("attack");
		return Leave;
	}
}
