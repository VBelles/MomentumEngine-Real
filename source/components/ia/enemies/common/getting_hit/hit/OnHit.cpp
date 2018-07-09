#include "mcv_platform.h"
#include "OnHit.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_hitboxes.h"
#include "components/comp_give_power.h"

OnHit::OnHit(Enemy* enemy, std::string animation) :
	enemy(enemy),
	animation(animation) {
}

int OnHit::execAction(float delta) {
	AttackInfo& attackInfo = enemy->receivedAttack;
	if (enemy->hasSuperArmor()) {
		enemy->superArmorAmount -= attackInfo.superArmorDamage;
	}
	if (enemy->blockTimer.elapsed() > enemy->blockTime) {
		enemy->isBlocking = false;
		enemy->blockingBroken = false;
	}
	if (enemy->isBlocking) {
		if (attackInfo.ignoresBlock) {
			enemy->hp -= attackInfo.damage;
			enemy->blockingBroken = true;
			enemy->getPower()->setStateMultiplier(1.f);
		}
	}
	else {
		enemy->hp -= attackInfo.damage;
		if (attackInfo.stun) {
			enemy->stunDuration = attackInfo.stun->duration;
		}
		else {
			enemy->stunDuration = 0;
		}
		enemy->stunTimer.reset();
	}
	if (enemy->hp <= 0 || enemy->getPower()->getPowerToGive() <= 0) {
		enemy->hp = 0;
		enemy->isBlocking = false;
		enemy->blockingBroken = false;
		enemy->superArmorAmount = 0;

		if (enemy->getPower()->getPowerToGive() > 0) {
			CEntity *entity = enemy->playerHandle;
			entity->sendMsg(TMsgGainPower{ CHandle(enemy->getPower()).getOwner(), enemy->getPower()->getPowerToGive() });
			enemy->getPower()->setPowerToGive(0.f);
		}
	}
	enemy->getHitboxes()->disableAll();
	enemy->velocity = VEC3::Zero;
	enemy->getSkeleton()->executeAction(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();
	return Leave;
}
