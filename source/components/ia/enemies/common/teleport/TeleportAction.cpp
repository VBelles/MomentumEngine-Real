#include "mcv_platform.h"
#include "TeleportAction.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_collider.h"

REGISTER_BTACTION("TeleportAction", TeleportAction);

TeleportAction::TeleportAction(Enemy* enemy, std::string animation) :
	enemy(enemy),
	animation(animation) {
}

int TeleportAction::execAction(float delta) {
	if (enemy->animationTimer.elapsed() < enemy->getSkeleton()->getAnimationDuration(animation)) {
		return Stay;
	}
	else {
		enemy->getCollider()->controller->setFootPosition(PhysxUtils::toPxtendedVec3(enemy->spawnPosition));
		return Leave;
	}
}

void TeleportAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}