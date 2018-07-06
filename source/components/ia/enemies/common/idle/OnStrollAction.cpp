#include "mcv_platform.h"
#include "OnStrollAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

OnStrollAction::OnStrollAction(Enemy* enemy, std::string animation) :
	enemy(enemy),
	animation(animation) {
}

int OnStrollAction::execAction(float delta) {
	enemy->getSkeleton()->blendCycle(animation, 0.1f, 0.1f);
	float randX = -enemy->strollRadius + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * enemy->strollRadius)));
	float randZ = -enemy->strollRadius + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * enemy->strollRadius)));

	VEC3 position = enemy->spawnPosition + VEC3(randX, 0.f, randZ);
	while (!enemy->navmesh->existsConnection(enemy->getTransform()->getPosition(), enemy->spawnPosition + VEC3(randX, 0.f, randZ))) {
		randX = -enemy->strollRadius + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * enemy->strollRadius)));
		randZ = -enemy->strollRadius + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * enemy->strollRadius)));
		position = enemy->spawnPosition + VEC3(randX, 0.f, randZ);
	}

	enemy->smoothPath = enemy->navmesh->getSmoothPath(enemy->getTransform()->getPosition(), position);
	enemy->currentPathPoint = 0;
	return Leave;
}
