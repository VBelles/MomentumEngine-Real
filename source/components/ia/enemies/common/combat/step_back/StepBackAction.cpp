#include "mcv_platform.h"
#include "StepBackAction.h"
#include "components/ia/enemies/Enemy.h"
#include "skeleton/comp_skeleton.h"
#include "components/comp_transform.h"

REGISTER_BTACTION("StepBackAction", StepBackAction);

StepBackAction::StepBackAction() {
	type = "StepBackAction";
}

StepBackAction::StepBackAction(Enemy* enemy, std::string animation, float speed) :
	StepBackAction() {
	this->enemy = enemy;
	this->animation = animation;
	this->speed = speed;
}

int StepBackAction::execAction(float delta) {
	enemy->updateGravity(delta);
	if (enemy->animationTimer.elapsed() < enemy->getSkeleton()->getAnimationDuration(animation)
		&& enemy->stepBackDistanceMoved < enemy->stepBackDistance) {
		enemy->stepBackDistanceMoved += speed * delta;
		VEC3 stepBackMovement = -enemy->getTransform()->getFront() * speed * delta;
		VEC3 pos = enemy->getTransform()->getPosition();

		if (enemy->navMeshQuery) {
			if (enemy->navMeshQuery->existsConnection(pos, pos + stepBackMovement)
				&& !enemy->navMeshQuery->raycast2D(pos, pos + stepBackMovement)) {
				enemy->deltaMovement += stepBackMovement;
			}
			else {
				return Leave;
			}
		}
		else {
			enemy->deltaMovement += stepBackMovement;
		}

		return Stay;
	}
	return Leave;
}

void StepBackAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
	speed = j.value("speed", speed);
}

void StepBackAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
	ImGui::DragFloat("Speed", &speed, 0.1f, 0.0f, 500.0f);
}