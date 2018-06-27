#include "mcv_platform.h"
#include "GrabbedAction.h"

#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"

GrabbedAction::GrabbedAction(Enemy* enemy): enemy(enemy) {
}

int GrabbedAction::execAction(float delta) {
	if (enemy->timer.elapsed() >= enemy->grabbedDuration) {
		enemy->getCollider()->create();
		enemy->getSkeleton()->setTimeFactor(1);
		return Leave;
	}
	return Stay;
}
