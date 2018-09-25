#include "mcv_platform.h"
#include "comp_effect.h"
#include "entity/entity_parser.h"
#include "components/comp_transform.h"
#include "entity/common_msgs.h"


DECL_OBJ_MANAGER("effect", TCompEffect);

void TCompEffect::debugInMenu() {
}

void TCompEffect::registerMsgs() {
	DECL_MSG(TCompEffect, TMsgEntityCreated, onEntityCreated);
}

void TCompEffect::load(const json& j, TEntityParseContext& ctx) {
	for (auto& jEffect : j["particles"]) {
		particleSystems.push_back(loadParticleEffect(jEffect));
	}
	for (auto& jEffect : j["sounds"]) {
		soundSystems.push_back(loadSoundEffect(jEffect));
	}
}

TCompEffect::ParticlesEffect TCompEffect::loadParticleEffect(const json& jEffect) {
	ParticlesEffect effect;
	for (auto& jEvent : jEffect["events"]) {
		EffectEvent event;
		event.time = jEvent.value("time", event.time);
		event.action = jEvent.value("action", event.action);
		effect.events.push_back(event);
	}
	return effect;
}
TCompEffect::SoundEffect TCompEffect::loadSoundEffect(const json& jEffect) {
	SoundEffect effect;
	for (auto& jEvent : jEffect["events"]) {
		EffectEvent event;
		event.time = jEvent.value("time", event.time);
		event.action = jEvent.value("action", event.action);
		effect.events.push_back(event);
	}
	return effect;
}

void TCompEffect::onEntityCreated(const TMsgEntityCreated & msg) {

}

void TCompEffect::update(float delta) {
	if (!running) return;
	float time = timer.elapsed();
	for (auto& effect : soundSystems) {
		for (auto& event : effect.events) {
			if (!event.consumed && event.time >= time) {
				event.consumed = true;
				if (event.action == "start") {
					//launchSoundSystem(event.data);
				}
			}
		}
	}
}

void TCompEffect::start() {
	running = true;
	/*for (auto& frameData : effectsFrameData) {
		frameData.consumed = false;
	}*/
	timer.reset();
}

void TCompEffect::stop() {
	running = false;
}


