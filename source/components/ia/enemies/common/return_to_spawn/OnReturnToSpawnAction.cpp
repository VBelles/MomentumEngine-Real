#include "mcv_platform.h"
#include "OnReturnToSpawnAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

REGISTER_BTACTION("OnReturnToSpawnAction", OnReturnToSpawnAction);

OnReturnToSpawnAction::OnReturnToSpawnAction(Enemy* enemy, std::string animation) :
	enemy(enemy),
	animation(animation) {
}

int OnReturnToSpawnAction::execAction(float delta) {
	enemy->smoothPath.clear();
	enemy->currentPathPoint = 0;

	if (enemy->navMeshQuery) {
		enemy->smoothPath = enemy->navMeshQuery->getSmoothPath(enemy->getTransform()->getPosition(), enemy->spawnPosition);
	}
	else {
		enemy->smoothPath.push_back(enemy->spawnPosition);
	}

	enemy->getSkeleton()->blendCycle(animation, 0.1f, 0.1f);
	return Leave;
}

void OnReturnToSpawnAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}