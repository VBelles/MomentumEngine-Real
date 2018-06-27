#include "mcv_platform.h"
#include "HorizontalLaunchedAction.h"

#include "components/comp_collider.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"

HorizontalLaunchedAction::HorizontalLaunchedAction(Enemy* enemy) : enemy(enemy) {
}

int HorizontalLaunchedAction::execAction(float delta) {
	enemy->updateGravity(delta);
	enemy->deltaMovement += enemy->velocity * delta;
	if (enemy->grounded || enemy->getTransform()->getPosition().y + enemy->deltaMovement.y - enemy->initialLaunchPos.y <= 0.f) {
		enemy->velocity.x = 0;
		enemy->velocity.z = 0;
		enemy->timer.reset();
		return Leave;
	}
	else {
		return Stay;
	}
}
