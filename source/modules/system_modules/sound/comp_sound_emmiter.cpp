#include "mcv_platform.h"
#include "comp_sound_emmiter.h"
#include "components/comp_transform.h"

using namespace FMOD;

DECL_OBJ_MANAGER("sound_emitter", TCompSoundEmitter);


void TCompSoundEmitter::registerMsgs() {
	DECL_MSG(TCompSoundEmitter, TMsgAllScenesCreated, onAllScenesCreated);
	DECL_MSG(TCompSoundEmitter, TMsgEntityDestroyed, onDestroyed);
}

void TCompSoundEmitter::debugInMenu() {
	if (ImGui::Button("Play")) {
		play();
	}
	if (ImGui::Button("Pause")) {
		pause();
	}
	if (ImGui::Button("Resume")) {
		resume();
	}
	if (ImGui::Button("Release")) {
		release();
	}
	ImGui::Text("Type: %s", is3D ? "3D" : "Surround");
	ImGui::Checkbox("Stop fade out", &stopFadeOut);
	ImGui::Text("Events: %d", eventInstances.size());
	int instanceCount = 0;
	eventDescriptor->getInstanceCount(&instanceCount);
	ImGui::Text("Resource events instanced: %d", instanceCount);
	ImGui::Text("Target: %s", transformHandle.isValid() ? target.empty()
		? "Own transform" : target.c_str()
		: "No target");
	ImGui::DragFloat3("Offset", &offset.x);
}

void TCompSoundEmitter::load(const json& j, TEntityParseContext& ctx) {
	eventResource = j.value("event", eventResource);
	releaseOnStop = j.value("release_on_stop", releaseOnStop);
	multiInstance = j.value("multi_instance", multiInstance);
	target = j.value("target", "");
	offset = j.count("offset") ? loadVEC3(j["offset"]) : offset;
	playOnStart = j.value("play", playOnStart);
	assert(res == FMOD_OK);
}

void TCompSoundEmitter::update(float delta) {
	FMOD_3D_ATTRIBUTES* attributes = nullptr;
	if (transformHandle.isValid()) {
		TCompTransform* transform = transformHandle;
		worldAttributes.position = toFMODVector(transform->getPosition() + offset);
		worldAttributes.velocity = { 0.f, 0.f, 0.f };
		worldAttributes.up = toFMODVector(transform->getUp());
		worldAttributes.forward = toFMODVector(transform->getFront());
		attributes = &worldAttributes;
	}
	// Update each instance's world attributes
	for (auto it = eventInstances.begin(); it != eventInstances.end();) {
		if ((*it)->isValid()) {
			// Update event instance
			(*it)->set3DAttributes(attributes);
			++it;
		}
		else {
			// Invalid event instance, remove it
			eventInstances.erase(it);
		}
	}
}

void TCompSoundEmitter::onAllScenesCreated(const TMsgAllScenesCreated&) {
	FMOD_RESULT res = EngineSound.getSystem()->getEvent(eventResource.c_str(), &eventDescriptor);
	eventDescriptor->is3D(&is3D);
	// Get target transform or its own transform
	targetHandle = !target.empty() ? getEntityByName(target) : CHandle(this).getOwner();
	transformHandle = static_cast<CEntity*>(targetHandle)->get<TCompTransform>();
	if (!is3D && transformHandle.isValid()) {
		dbg("WARNING: %s is NOT 3d but its transform is valid", eventResource.c_str());
	}
	if (playOnStart) {
		play();
	}
}


void TCompSoundEmitter::onDestroyed(const TMsgEntityDestroyed&) {
	release();
}

void TCompSoundEmitter::play() {
	if (!multiInstance) {
		// Clears previous instances
		for (auto eventInstance : eventInstances) {
			eventInstance->stop(stopFadeOut ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE);
			eventInstance->release();
		}
		eventInstances.clear();
	}
	// Create instance
	Studio::EventInstance* eventInstance = nullptr;
	eventDescriptor->createInstance(&eventInstance);
	eventInstance->start();
	if (releaseOnStop) {
		// Release on stop
		eventInstance->release();
	}
	eventInstances.push_back(eventInstance);
}

void TCompSoundEmitter::release() {
	for (auto eventInstance : eventInstances) {
		eventInstance->stop(stopFadeOut ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE);
		eventInstance->release();
	}
	eventInstances.clear();
}

void TCompSoundEmitter::pause() {
	for (auto eventInstance : eventInstances) {
		eventInstance->setPaused(true);
	}
}

void TCompSoundEmitter::resume() {
	for (auto eventInstance : eventInstances) {
		eventInstance->setPaused(false);
	}
}

bool TCompSoundEmitter::isEventEmitting(const FMOD::Studio::EventInstance* eventInstance) const {
	if (!eventInstance || !eventInstance->isValid()) {
		return false;
	}
	FMOD_STUDIO_PLAYBACK_STATE playbackState;
	eventInstance->getPlaybackState(&playbackState);
	return playbackState != FMOD_STUDIO_PLAYBACK_STOPPED;
}

bool TCompSoundEmitter::isEmitting() {
	return !eventInstances.empty();
}

