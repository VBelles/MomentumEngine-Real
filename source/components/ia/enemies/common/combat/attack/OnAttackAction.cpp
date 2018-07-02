#include "mcv_platform.h"
#include "OnAttackAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

OnAttackAction::OnAttackAction(Enemy* enemy, std::string animation, std::string attack):
	enemy(enemy),
	animation(animation),
	attack(attack) {
}

int OnAttackAction::execAction(float delta) {
	VEC3 playerPosition = enemy->getPlayerTransform()->getPosition();
	enemy->rotateTowards(delta, playerPosition, enemy->rotationSpeed);
	enemy->getSkeleton()->blendCycle(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();
	enemy->currentAttack = attack;
	return Leave;
}
