#include "mcv_platform.h"
#include "OnVerticalLaunchAction.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_hitboxes.h"

REGISTER_BTACTION("OnVerticalLaunchAction", OnVerticalLaunchAction);

OnVerticalLaunchAction::OnVerticalLaunchAction() {
	type = "OnVerticalLaunchAction";
}

OnVerticalLaunchAction::OnVerticalLaunchAction(Enemy* enemy, std::string animation, std::string attack) :
	OnVerticalLaunchAction() {
	this->enemy = enemy;
	this->animation = animation;
	this->attack = attack;
}

int OnVerticalLaunchAction::execAction(float delta) {
	enemy->getSkeleton()->executeAction(animation, 0.1f, 0.1f);
	enemy->suspensionDuration = enemy->receivedAttack.verticalLauncher->suspensionDuration;
	enemy->velocity = enemy->receivedAttack.verticalLauncher->velocity;
	enemy->initialLaunchPos = enemy->getTransform()->getPosition();
	enemy->currentAttack = attack;
	enemy->getHitboxes()->enable(enemy->attacks[attack].hitboxName);
	enemy->isBlocking = false;
	enemy->blockingBroken = false;
	return Leave;
}

void OnVerticalLaunchAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
	attack = j.value("attack", attack);
}

void OnVerticalLaunchAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
	ImGui::Text("Attack: %s\n", attack.c_str());
}