#include "mcv_platform.h"
#include "OnAttackAction.h"
#include "components/ia/enemies/dreidel/Dreidel.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"

OnAttackAction::OnAttackAction(Dreidel* dreidel): dreidel(dreidel) {
}

int OnAttackAction::execAction(float delta) {
	dreidel->updateGravity(delta);
	dreidel->rotateTowards(delta, dreidel->getPlayerTransform()->getPosition(), dreidel->rotationSpeed);
	if (dreidel->attackTimer.elapsed() < dreidel->attackCooldown) {
		dreidel->setCurrent(nullptr);
	}
	else {
		dreidel->hasAttacked = false;
		dreidel->attackTimer.reset();
		dreidel->getSkeleton()->executeAction(2, 0.2f, 0.2f);
	}
	return Leave;
}
