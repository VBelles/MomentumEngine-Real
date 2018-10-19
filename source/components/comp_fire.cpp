#include "mcv_platform.h"
#include "comp_fire.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"
#include "modules/system_modules/particles/comp_particles.h"
#include "components/comp_light_point.h"
#include "components/lights/comp_light_flicker.h"
#include "components/player/attack_info.h"


DECL_OBJ_MANAGER("fire", TCompFire);

void TCompFire::debugInMenu() {
}

void TCompFire::registerMsgs() {
	DECL_MSG(TCompFire, TMsgEntityCreated, onEntityCreated);
	DECL_MSG(TCompFire, TMsgTriggerEnter, onPlayerEnter);
	DECL_MSG(TCompFire, TMsgTriggerExit, onPlayerExit);
	DECL_MSG(TCompFire, TMsgAttackHit, onAttackHit);

}

void TCompFire::load(const json& j, TEntityParseContext& ctx) {
	if (j.count("fire")) {
		for (std::string particleId : j["fire"]) {
			fireParticles.push_back(particleId);
		}
	}
	if (j.count("smoke")) {
		for (std::string particleId : j["smoke"]) {
			smokeParticles.push_back(particleId);
		}
	}
	time = j.value("time", time);
}

void TCompFire::onEntityCreated(const TMsgEntityCreated& msg) {
	particlesHandle = get<TCompParticles>();
	lightHandle = get<TCompLightPoint>();
	lightFlicker = get<TCompLightFlicker>();
	assert(particlesHandle.isValid());
	assert(lightHandle.isValid());
	assert(lightFlicker.isValid());
	hasFire = true;
	playerOnFire = false;
}

void TCompFire::onPlayerEnter(const TMsgTriggerEnter& msg) {
	if (static_cast<CEntity*>(msg.h_other_entity)->getName() != PLAYER_NAME) return;
	dbg("Player enter\n");
	// Kill fire
	//dbg("kill fire\n");
	playerOnFire = true;
	killFire();
}

void TCompFire::onPlayerExit(const TMsgTriggerExit& msg) {
	if (static_cast<CEntity*>(msg.h_other_entity)->getName() != PLAYER_NAME) return;
	//dbg("Player exit\n");
	timer.reset();
	playerOnFire = false;
}

void TCompFire::onAttackHit(const TMsgAttackHit& msg) {
	CEntity* attackerEntity = msg.attacker;
	killFire();
	timer.reset();
}

void TCompFire::killFire() {
	getLight()->setIntensity(0.f);
	getLightFlicker()->setActive(false);
	for (auto& particleId : fireParticles) {
		getParticles()->kill(particleId);
	}
	// Launch smoke
	if (hasFire) {
		//dbg("launch smoke\n");
		for (auto& particleId : smokeParticles) {
			getParticles()->launch(particleId);
		}
		hasFire = false;
	}
}

void TCompFire::update(float delta) {
	if (!playerOnFire && !hasFire && timer.elapsed() >= time) {
		hasFire = true;
		// Launch fire
		//dbg("launch fire\n");
		getLightFlicker()->setActive(true);
		for (auto& particleId : fireParticles) {
			getParticles()->launch(particleId);
		}
	}
}

TCompParticles* TCompFire::getParticles() {
	return particlesHandle;
}

TCompLightPoint* TCompFire::getLight() {
	return lightHandle;
}

TCompLightFlicker* TCompFire::getLightFlicker() {
	return lightFlicker;
}


