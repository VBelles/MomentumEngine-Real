#include "mcv_platform.h"
#include "HorizontalLaunchedAction.h"
#include "components/comp_transform.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("HorizontalLaunchedAction", HorizontalLaunchedAction);

HorizontalLaunchedAction::HorizontalLaunchedAction() {
	type = "HorizontalLaunchedAction";
}

HorizontalLaunchedAction::HorizontalLaunchedAction(Enemy* enemy) :
	HorizontalLaunchedAction() {
	this->enemy = enemy;
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

void HorizontalLaunchedAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}