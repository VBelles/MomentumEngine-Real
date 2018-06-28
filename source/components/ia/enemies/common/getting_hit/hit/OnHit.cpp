#include "mcv_platform.h"
#include "OnHit.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"

OnHit::OnHit(Enemy* enemy, std::string animation) :
	enemy(enemy),
	animation(animation) {
}

int OnHit::execAction(float delta) {
	AttackInfo& attackInfo = enemy->receivedAttack;
	if (enemy->hasSuperArmor()) {
		enemy->superArmorAmount -= attackInfo.superArmorDamage;
	}
	if (enemy->isBlocking) {
		if (attackInfo.ignoresBlock) {
			enemy->hp -= attackInfo.damage;
			enemy->blockingBroken = true;
		}
	}
	else {
		enemy->hp -= attackInfo.damage;
	}
	enemy->getSkeleton()->executeAction(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();
	if (attackInfo.stun) {
		enemy->stunDuration = attackInfo.stun->duration;
	}
	else {
		enemy->stunDuration = 0;
	}
	enemy->stunTimer.reset();
	return Leave;
}
