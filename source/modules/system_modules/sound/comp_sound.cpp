#include "mcv_platform.h"
#include "comp_sound.h"

using namespace FMOD;

DECL_OBJ_MANAGER("sound", TCompSound);


void TCompSound::registerMsgs() {
	DECL_MSG(TCompSound, TMsgAllScenesCreated, onAllScenesCreated);
	DECL_MSG(TCompSound, TMsgEntityDestroyed, onDestroyed);
}

void TCompSound::debugInMenu() {
	
}

void TCompSound::load(const json& j, TEntityParseContext& ctx) {
	assert(j.count("events"));
	for (auto& jEvent : j["events"]) {
		Sound sound;
		std::string id = jEvent.value("id", "");
		assert(!id.empty());
		sound.path = jEvent.value("path", sound.path);
		sound.following = jEvent.value("following", sound.following);
		sound.multiInstancing = jEvent.value("multi_instancing", sound.multiInstancing);
		sound.stopFadeOut = jEvent.value("stop_fade_out", sound.stopFadeOut);
		events[id] = sound;
	}
}

void TCompSound::onAllScenesCreated(const TMsgAllScenesCreated&) {
	for (auto& p : events) {
		auto& sound = p.second;
		EngineSound.getSystem()->getEvent(sound.path.c_str(), &sound.eventDescriptor);
	}
}


void TCompSound::onDestroyed(const TMsgEntityDestroyed&) {
	stop();
}

void TCompSound::update(float delta) {
	TCompTransform* transform = get<TCompTransform>();

	for (auto& p : events) {
		auto& sound = p.second;
		auto& eventInstances = sound.eventInstances;
		auto it = eventInstances.begin();
		while (it != eventInstances.end()) {
			auto& eventInstance = *it;
			if (!eventInstance || !eventInstance->isValid()) { // Event has been released
				it = eventInstances.erase(it);
			}
			else {
				if (sound.following) {
					FMOD_3D_ATTRIBUTES attributes = toFMODAttributes(*transform);
					eventInstance->set3DAttributes(&attributes);
				}
				++it;
			}
		}
	}
}

void TCompSound::play(std::string event) {
	auto it = events.find(event);
	if (it == events.end())
		return;
	auto& sound = it->second;
	if (!sound.multiInstancing) {
		stop(event);
	}
	Studio::EventInstance* eventInstance = nullptr;
	sound.eventDescriptor->createInstance(&eventInstance);
	sound.eventInstances.push_back(eventInstance);
}

void TCompSound::stop(std::string event) {
	auto it = events.find(event);
	if (it == events.end())
		return;
	auto& sound = it->second;
	for (auto& eventInstance : sound.eventInstances) {
		eventInstance->stop(sound.stopFadeOut ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE);
	}
	sound.eventInstances.clear();
}

void TCompSound::stop() {
	for (auto& p : events) {
		auto& sound = p.second;
		for (auto& eventInstance : sound.eventInstances) {
			eventInstance->stop(sound.stopFadeOut ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE);
		}
		sound.eventInstances.clear();
	}
}




