#include "mcv_platform.h"
#include "GrabAction.h"

#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"

GrabAction::GrabAction(Enemy* enemy): enemy(enemy) {
}

int GrabAction::execAction(float delta) {
	if (enemy->timer.elapsed() >= enemy->grabbedDuration) {
		enemy->getCollider()->create(); //TODO enable/disable
		return Leave;
	}
	return Stay;
}
