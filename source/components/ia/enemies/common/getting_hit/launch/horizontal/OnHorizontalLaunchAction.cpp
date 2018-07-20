#include "mcv_platform.h"
#include "OnHorizontalLaunchAction.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("OnHorizontalLaunchAction", OnHorizontalLaunchAction);

OnHorizontalLaunchAction::OnHorizontalLaunchAction() {
	type = "OnHorizontalLaunchAction";
}

OnHorizontalLaunchAction::OnHorizontalLaunchAction(Enemy* enemy, std::string animation) :
	OnHorizontalLaunchAction() {
	this->enemy = enemy;
	this->animation = animation;
}

int OnHorizontalLaunchAction::execAction(float delta) {
	enemy->getSkeleton()->executeAction(animation, 0.1f, 0.1f);
	enemy->stunDuration = enemy->receivedAttack.horizontalLauncher->suspensionDuration;
	enemy->velocity = enemy->receivedAttack.horizontalLauncher->velocity;
	enemy->initialLaunchPos = enemy->getTransform()->getPosition();
	enemy->isBlocking = false;
	enemy->blockingBroken = false;
	return Leave;
}

void OnHorizontalLaunchAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
}