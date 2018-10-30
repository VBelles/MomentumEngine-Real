#include "mcv_platform.h"
#include "comp_sound.h"

using namespace FMOD;

DECL_OBJ_MANAGER("sound", TCompSound);


void TCompSound::registerMsgs() {
	DECL_MSG(TCompSound, TMsgEntityCreated, onEntityCreated);
	DECL_MSG(TCompSound, TMsgEntityDestroyed, onDestroyed);
}

void TCompSound::debugInMenu() {
	for (auto& p : events) {
		auto& sound = p.second;
		ImGui::Text("%s - %s", p.first.c_str(), sound.path.c_str());
		ImGui::Text("Multi instancing: %s", sound.multiInstancing ? "yes" : "no");
		ImGui::Text("Following: %s", sound.following ? "yes" : "no");
		ImGui::Text("Stop fade out: %s", sound.stopFadeOut ? "yes" : "no");
		ImGui::Text("Instances: %d", sound.eventInstances.size());
		if (ImGui::Button("Play")) {
			play(p.first);
		}
		if (ImGui::Button("Stop")) {
			stop(p.first);
		}
	}
	if (ImGui::Button("Stop all")) {
		stop();
	}
}

void TCompSound::load(const json& j, TEntityParseContext& ctx) {
	assert(j.count("events"));
	bool is3D = j.value("3D", true);
	bool playOnStart = j.value("play", false);
	float maxDistance = j.value("max_distance", -1.f);
	for (auto& jEvent : j["events"]) {
		Sound sound;
		sound.id = jEvent.value("id", "");
		assert(!sound.id.empty());
		sound.path = jEvent.value("path", sound.path);
		sound.following = jEvent.value("following", sound.following);
		sound.multiInstancing = jEvent.value("multi_instancing", sound.multiInstancing);
		sound.stopFadeOut = jEvent.value("stop_fade_out", sound.stopFadeOut);
		sound.is3D = jEvent.value("3D", is3D);
		sound.playOnStart = jEvent.value("play", playOnStart);
		sound.maxDistance = jEvent.value("max_distance", maxDistance);
		events[sound.id] = sound;
	}
}

void TCompSound::onEntityCreated(const TMsgEntityCreated&) {
	for (auto& p : events) {
		auto& sound = p.second;
		EngineSound.getSystem()->getEvent(sound.path.c_str(), &sound.eventDescription);
		bool is3D = false;
		sound.eventDescription->is3D(&is3D);
		if (sound.is3D != is3D) {
			dbg("Warning: %s declared as %s but the component declares it as %s\n",
				sound.path.c_str(), is3D ? "3D" : "not 3D", sound.is3D ? "3D" : "not 3D");
		}
		if (sound.playOnStart) {
			play(sound.id);
		}
	}
}


void TCompSound::onDestroyed(const TMsgEntityDestroyed&) {
	//dbg("Component destroyed\n");
	stop();
}

void TCompSound::update(float delta) {
	TCompTransform* transform = get<TCompTransform>();
	for (auto& p : events) {
		auto& sound = p.second;
		sound.eventInstances.erase(std::remove_if(sound.eventInstances.begin(), sound.eventInstances.end(), [&](FMOD::Studio::EventInstance*& eventInstance) -> bool {
			// Event has been released
			if (!eventInstance || !eventInstance->isValid()) {
				return true;
			}
			if (transform && sound.is3D && sound.maxDistance > 0.f) {
				float maxDistanceSquared = sound.maxDistance * sound.maxDistance;
				float distanceSquared = VEC3::DistanceSquared(EngineSound.getListenerPosition(), transform->getPosition());
				if (distanceSquared > maxDistanceSquared) {
					//dbg("Distance: %f/%f\n", maxDistance, sqrt(distanceSquared));
					FarEvent farEvent;
					farEvent.timer.reset();
					farEvent.maxDistanceSquared = maxDistanceSquared;
					farEvent.id = sound.id;
					eventInstance->getTimelinePosition(&farEvent.timeline);
					eventInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE);
					sound.farEvents.push_back(farEvent);
					return true;
				}
			}
			// Update following event
			if (transform && sound.following) {
				FMOD_3D_ATTRIBUTES attributes = toFMODAttributes(*transform);
				eventInstance->set3DAttributes(&attributes);
			}
			return false;
		}), sound.eventInstances.end());

		if (transform) {
			sound.farEvents.erase(std::remove_if(sound.farEvents.begin(), sound.farEvents.end(), [&](auto& farEvent) -> bool {
				float distanceSquared = VEC3::DistanceSquared(EngineSound.getListenerPosition(), transform->getPosition());
				if (distanceSquared <= farEvent.maxDistanceSquared) {
					if (play(sound.id)) {
						auto eventInstance = sound.eventInstances[sound.eventInstances.size() - 1];
						int timelineElapse = (int)(farEvent.timer.elapsed() * 1000.f);
						eventInstance->setTimelinePosition(timelineElapse);
					}
					return true;
				}
				return false;
			}), sound.farEvents.end());
		}

	}
}

bool TCompSound::play(std::string event) {
	auto it = events.find(event);
	if (it == events.end())
		return true;
	auto& sound = it->second;
	if (!sound.multiInstancing) {
		stop(event);
	}
	Studio::EventInstance* eventInstance = nullptr;
	sound.eventDescription->createInstance(&eventInstance);

	TCompTransform* transform = get<TCompTransform>();
	if (sound.is3D && transform) {
		FMOD_3D_ATTRIBUTES attributes = toFMODAttributes(*transform);
		eventInstance->set3DAttributes(&attributes);
	}
	eventInstance->start();
	eventInstance->release();
	sound.eventInstances.push_back(eventInstance);
	return false;
}

void TCompSound::stop(std::string event) {
	auto it = events.find(event);
	if (it == events.end())
		return;
	auto& sound = it->second;
	for (auto& eventInstance : sound.eventInstances) {
		eventInstance->stop(sound.stopFadeOut ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE);
	}
	sound.farEvents.clear();
	sound.eventInstances.clear();
}

void TCompSound::stop() {
	for (auto& p : events) {
		auto& sound = p.second;
		for (auto& eventInstance : sound.eventInstances) {
			eventInstance->stop(sound.stopFadeOut ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE);
		}
		sound.eventInstances.clear();
		sound.farEvents.clear();
	}
}

bool TCompSound::isPlaying(std::string event) {
	auto it = events.find(event);
	if (it == events.end())
		return false;
	auto& sound = it->second;
	return !sound.eventInstances.empty();
}




