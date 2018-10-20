#include "mcv_platform.h"
#include "OnStrollAction.h"
#include "components/ia/enemies/Enemy.h"
#include "components/player/comp_player_model.h"

REGISTER_BTACTION("OnStrollAction", OnStrollAction);

OnStrollAction::OnStrollAction() {
	type = "OnStrollAction";
}

OnStrollAction::OnStrollAction(Enemy* enemy, std::string animation) :
	OnStrollAction() {
	this->enemy = enemy;
	this->animation = animation;
}

int OnStrollAction::execAction(float delta) {
	enemy->smoothPath.clear();
	enemy->currentPathPoint = 0;

	float randX = -enemy->strollRadius + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * enemy->strollRadius)));
	float randZ = -enemy->strollRadius + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * enemy->strollRadius)));
	VEC3 position = enemy->spawnPosition + VEC3(randX, 0.f, randZ);

	if (enemy->navMeshQuery) {
		while (!enemy->navMeshQuery->existsConnection(enemy->getTransform()->getPosition(), enemy->spawnPosition + VEC3(randX, 0.f, randZ))) {
			randX = -enemy->strollRadius + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * enemy->strollRadius)));
			randZ = -enemy->strollRadius + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * enemy->strollRadius)));
			position = enemy->spawnPosition + VEC3(randX, 0.f, randZ);
		}

		enemy->smoothPath = enemy->navMeshQuery->getSmoothPath(enemy->getTransform()->getPosition(), position);
	}
	else {
		enemy->smoothPath.push_back(position);
	}

	if (!enemy->attackTarget.empty()) {
		CEntity* entity = enemy->getEntityHandle();
		enemy->getPlayerModel()->removeAttacker(entity->getName(), enemy->attackSlots);
		enemy->attackTarget = "";
	}

	enemy->getSkeleton()->blendCycle(animation, 0.1f, 0.1f);
	enemy->getSound()->play("stroll");
	return Leave;
}

void OnStrollAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}

void OnStrollAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
}
