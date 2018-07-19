#include "mcv_platform.h"
#include "OnTackleAttackAction.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_transform.h"

REGISTER_BTACTION("OnTackleAttackAction", OnTackleAttackAction);

OnTackleAttackAction::OnTackleAttackAction(Enemy* enemy, std::string animation, std::string attack):
	enemy(enemy),
	animation(animation),
	attack(attack) {
}

int OnTackleAttackAction::execAction(float delta) {
	VEC3 playerPosition = enemy->getPlayerTransform()->getPosition();
	enemy->rotateTowards(delta, playerPosition, enemy->rotationSpeed);
	enemy->getSkeleton()->blendCycle(animation, 0.1f, 0.1f);
	enemy->timer.reset();
	enemy->currentAttack = attack;
	return Leave;
}

void OnTackleAttackAction::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
	attack = j.value("attack", attack);
}