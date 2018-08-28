#include "mcv_platform.h"
#include "OnHit.h"
#include "skeleton/comp_skeleton.h"
#include "components/ia/enemies/Enemy.h"
#include "components/comp_hitboxes.h"
#include "components/comp_give_power.h"

REGISTER_BTACTION("OnHit", OnHit);

OnHit::OnHit() {
	type = "OnHit";
}

OnHit::OnHit(Enemy* enemy, std::string animation) :
	OnHit() {
	this->enemy = enemy;
	this->animation = animation;
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
			entity->sendMsg(TMsgGainPower{ enemy->getEntityHandle(), enemy->getPower()->getPowerToGive() });
			enemy->getPower()->setPowerToGive(0.f);
		}
	}
	enemy->gravityMultiplier = attackInfo.gravityMultiplier;
	enemy->getHitboxes()->disableAll();
	enemy->velocity = VEC3::Zero;
	enemy->getSkeleton()->executeAction(animation, 0.1f, 0.1f);
	enemy->animationTimer.reset();

	//Launch hit particles
	EngineParticles.launchSystem(particles, Particles::LaunchConfig{ enemy->getEntityHandle(), "", particlesOffset });

	return Leave;
}

void OnHit::load(IBehaviorTreeNew* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
	particles = j.value("particles", particles);
	particlesOffset = j.count("particles_offset") ? loadVEC3(j.value("particles_offset", "")) : particlesOffset;
}

void OnHit::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
}