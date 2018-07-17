#include "mcv_platform.h"
#include "OnHorizontalLaunchAction.h"
#include "components/comp_transform.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_hitboxes.h"

OnHorizontalLaunchAction::OnHorizontalLaunchAction(Enemy* enemy, std::string animation, std::string attack) :
	enemy(enemy),
	animation(animation),
	attack(attack) {
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
	//TODO aquí debería poner su gravedad a 0.6 de la normal
	return Leave;
}
