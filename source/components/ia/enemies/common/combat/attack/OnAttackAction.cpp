#include "mcv_platform.h"
#include "OnAttackAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

REGISTER_BTACTION("OnAttackAction", OnAttackAction);

OnAttackAction::OnAttackAction() {
	type = "OnAttackAction";
}

OnAttackAction::OnAttackAction(Enemy* enemy, std::string animation, std::string attack) : OnAttackAction() {
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
	return Leave;
}

void OnAttackAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
	attack = j.value("attack", attack);
}
