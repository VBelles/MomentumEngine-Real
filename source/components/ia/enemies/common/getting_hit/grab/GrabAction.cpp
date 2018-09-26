#include "mcv_platform.h"
#include "GrabAction.h"
#include "components/comp_collider.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("GrabAction", GrabAction);

GrabAction::GrabAction() {
	type = "GrabAction";
}

GrabAction::GrabAction(Enemy* enemy) :
	GrabAction() {
	this->enemy = enemy;
}

int GrabAction::execAction(float delta) {
	if (enemy->timer.elapsed() >= enemy->grabbedDuration) {
		enemy->getCollider()->create(); //TODO enable/disable
		return Leave;
	}
	return Stay;
}

void GrabAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);
}