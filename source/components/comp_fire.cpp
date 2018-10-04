#include "mcv_platform.h"
#include "comp_fire.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"
#include "modules/system_modules/particles/comp_particles.h"
#include "components/comp_light_point.h"


DECL_OBJ_MANAGER("fire", TCompFire);

void TCompFire::debugInMenu() {
}

void TCompFire::registerMsgs() {
	DECL_MSG(TCompFire, TMsgEntityCreated, onEntityCreated);
	DECL_MSG(TCompFire, TMsgTriggerEnter, onPlayerEnter);
	DECL_MSG(TCompFire, TMsgTriggerExit, onPlayerExit);

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
	assert(particlesHandle.isValid());
}

void TCompFire::onPlayerEnter(const TMsgTriggerEnter& msg) {
	if (timer.elapsed() > 1.f && static_cast<CEntity*>(msg.h_other_entity)->getName() != PLAYER_NAME) return;
	dbg("Player enter\n");
	// Kill fire
	dbg("kill fire\n");
	getLight()->setIntensity(0.f);
	for (auto& particleId : fireParticles) {
		getParticles()->kill(particleId);
	}
	// Launch smoke
	if (hasFire) {
		dbg("launch smoke\n");
		for (auto& particleId : smokeParticles) {
			getParticles()->launch(particleId);
		}
	}
	hasFire = false;
	playerOnFire = true;
}

void TCompFire::onPlayerExit(const TMsgTriggerExit& msg) {
	if (static_cast<CEntity*>(msg.h_other_entity)->getName() != PLAYER_NAME) return;
	dbg("Player exit\n");
	timer.reset();
	playerOnFire = false;
}

void TCompFire::update(float delta) {
	if (!playerOnFire && !hasFire && timer.elapsed() >= time) {
		hasFire = true;
		// Launch fire
		dbg("launch fire\n");
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


