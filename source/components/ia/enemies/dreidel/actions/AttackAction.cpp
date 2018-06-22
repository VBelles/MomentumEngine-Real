#include "mcv_platform.h"
#include "AttackAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "components/comp_transform.h"
#include "components/comp_hitboxes.h"
#include "skeleton/comp_skeleton.h"

AttackAction::AttackAction(Dreidel* dreidel): dreidel(dreidel) {
}

int AttackAction::execAction(float delta) {
	VEC2 attack = dreidel->attacksFrameData["attack"];
	dreidel->updateGravity(delta);
	dreidel->rotateTowards(delta, dreidel->getPlayerTransform()->getPosition(), dreidel->rotationSpeed);
	if (dreidel->attackTimer.elapsed() < (dreidel->getSkeleton()->getAnimationDuration(2))) {
		if (!dreidel->hasAttacked && dreidel->attackTimer.elapsed() >= frames2sec(attack.x)) {
			dreidel->getHitboxes()->enable("attack");
			dreidel->hasAttacked = true;
		}
		else if (dreidel->hasAttacked && dreidel->attackTimer.elapsed() >= frames2sec(attack.x + attack.y)) {
			dreidel->getHitboxes()->disable("attack");
		}
		return Stay;
	}
	else {
		dreidel->getHitboxes()->disable("attack");
		return Leave;
	}
}
