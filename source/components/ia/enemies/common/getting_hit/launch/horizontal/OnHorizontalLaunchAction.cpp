#include "mcv_platform.h"
#include "OnHorizontalLaunchAction.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_hitboxes.h"

REGISTER_BTACTION("OnHorizontalLaunchAction", OnHorizontalLaunchAction);

OnHorizontalLaunchAction::OnHorizontalLaunchAction() {
	type = "OnHorizontalLaunchAction";
}

OnHorizontalLaunchAction::OnHorizontalLaunchAction(Enemy* enemy, std::string animation, std::string attack) :
	OnHorizontalLaunchAction() {
	this->enemy = enemy;
	this->animation = animation;
	this->attack = attack;
}

int OnHorizontalLaunchAction::execAction(float delta) {
	enemy->getSkeleton()->executeAction(animation, 0.1f, 0.1f);
	enemy->stunDuration = enemy->receivedAttack.horizontalLauncher->suspensionDuration;
	enemy->velocity = enemy->receivedAttack.horizontalLauncher->velocity;
	enemy->initialLaunchPos = enemy->getTransform()->getPosition();
	enemy->currentAttack = attack;
	enemy->getHitboxes()->enable(enemy->attacks[attack].hitboxName);
	enemy->isBlocking = false;
	enemy->blockingBroken = false;
	return Leave;
}

void OnHorizontalLaunchAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
	attack = j.value("attack", attack);
}

void OnHorizontalLaunchAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
	ImGui::Text("Attack: %s\n", attack.c_str());
}