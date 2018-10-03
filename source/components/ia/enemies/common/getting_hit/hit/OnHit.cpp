#include "mcv_platform.h"
#include "OnHit.h"
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
		else {
			enemy->getSound()->play("block");
		}
	}
	else {
		enemy->hp -= attackInfo.damage;
		if (attackInfo.stun) {
			enemy->stunDuration = attackInfo.stun->duration;
		}
		else {
			enemy->stunDuration = 0.f;
		}
		enemy->stunTimer.reset();
		enemy->getSound()->play("hit");
	}
	if (enemy->hp <= 0.f || enemy->getPower()->getPowerToGive() <= 0) {
		enemy->hp = 0.f;
		enemy->isBlocking = false;
		enemy->blockingBroken = false;
		enemy->superArmorAmount = 0.f;

		if (enemy->getPower()->getPowerToGive() > 0.f) {
			CEntity *entity = enemy->playerHandle;
			entity->sendMsg(TMsgGainPower{ enemy->getEntityHandle(), enemy->getPower()->getPowerToGive() });
			enemy->getPower()->setPowerToGive(0.f);
		}

		if (enemy->attacker != PLAYER_NAME) {
			enemy->onDisappear = true;
			enemy->resetCurrent();
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

void OnHit::load(IBehaviorTree* bt, const json& j) {
	enemy = dynamic_cast<Enemy*>(bt);
	assert(enemy);

	animation = j.value("animation", animation);
	particles = j.value("particles", particles);
	particlesOffset = j.count("particles_offset") ? loadVEC3(j.value("particles_offset", "")) : particlesOffset;
}

void OnHit::debugInMenu() {
	ImGui::Text("Animation: %s\n", animation.c_str());
}
