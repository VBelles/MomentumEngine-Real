#include "mcv_platform.h"
#include "OnAttackAction.h"
#include "components/ia/enemies/Enemy.h"

REGISTER_BTACTION("OnAttackAction", OnAttackAction);

OnAttackAction::OnAttackAction() {
	type = "OnAttackAction";
}

OnAttackAction::OnAttackAction(Enemy* enemy, std::string animation, std::string attack) :
	OnAttackAction() {
	this->enemy = enemy;
	this->animation = animation;
	this->attack = attack;
}

int OnAttackAction::execAction(float delta) {
	VEC3 playerPosition = enemy->getPlayerTransform()->getPosition();
	enemy->rotateTowards(delta, playerPosition, enemy->rotationSpeed);
	enemy->getSkeleton()->blendCycle(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();
	enemy->currentAttack = attack;
	auto& enemyAttack = enemy->attacks[attack];
	enemy->launchParticles(enemyAttack.particles);
	enemy->playSounds(enemyAttack.sounds);
	return Leave;
}

void OnAttackAction::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
	attack = j.value("attack", attack);
}

void OnAttackAction::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
	ImGui::Text("Attack: %s\n", attack.c_str());
}
