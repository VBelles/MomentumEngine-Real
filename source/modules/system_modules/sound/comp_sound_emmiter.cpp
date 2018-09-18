#include "mcv_platform.h"
#include "comp_sound_emmiter.h"
#include "resources/resources_manager.h"
#include "components/comp_transform.h"
#include <fmod_common.h>

DECL_OBJ_MANAGER("sound_emmiter", TCompSoundEmmiter);


void TCompSoundEmmiter::registerMsgs() {
	DECL_MSG(TCompSoundEmmiter, TMsgAllScenesCreated, onAllScenesCreated);
	DECL_MSG(TCompSoundEmmiter, TMsgEntityDestroyed, onDestroyed);
}

void TCompSoundEmmiter::debugInMenu() {
	if (ImGui::Checkbox("Emmiting", &emmiting)) {
		if (emmiting) {
			start();
		}
		else {
			stop();
		}
	}
	ImGui::Text("Target: %s", target.c_str());
	ImGui::DragFloat3("Offset", &offset.x);
}

void TCompSoundEmmiter::load(const json& j, TEntityParseContext& ctx) {
	target = j.value("target", "");
	offset = j.count("offset") ? loadVEC3(j["offset"]) : offset;
	assert(_core);
}

void TCompSoundEmmiter::update(float delta) {
	TCompTransform* transform = transformHandle;
	attributes.position = toFMODVector(transform->getPosition());
	attributes.velocity = { 0.f, 0.f, 0.f };
	attributes.up = toFMODVector(transform->getUp());
	attributes.forward = toFMODVector(transform->getFront());

	for (auto instance : eventInstances) {
		if (isEmmiting(instance)) {
			instance->set3DAttributes(&attributes);
		}
		else {
			instance->release();
		}
	}

}

void TCompSoundEmmiter::onAllScenesCreated(const TMsgAllScenesCreated&) {
	if (!target.empty()) {
		targetHandle = getEntityByName(target);
	}
	else {
		targetHandle = CHandle(this).getOwner();
	}
	transformHandle = static_cast<CEntity*>(targetHandle)->get<TCompTransform>();
}


void TCompSoundEmmiter::onDestroyed(const TMsgEntityDestroyed&) {

}

FMOD_RESULT F_CALLBACK MyCallback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE *event, void *parameters) {
	FMOD::Studio::EventInstance *instance = (FMOD::Studio::EventInstance *)event;

	if (type == FMOD_STUDIO_EVENT_CALLBACK_STOPPED) {
		// Handle event instance stop here
	}
	else if (type == FMOD_STUDIO_EVENT_CALLBACK_CREATE_PROGRAMMER_SOUND) {
		FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES* properties = (FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES *)parameters;

		// Handle programmer sound creation here
	}
	else if (type == FMOD_STUDIO_EVENT_CALLBACK_DESTROY_PROGRAMMER_SOUND) {
		FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES* properties = (FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES *)parameters;

		// Handle programmer sound destruction here
	}

	return FMOD_OK;
}

void TCompSoundEmmiter::emmit() {
	FMOD::Studio::EventInstance* eventInstance = EngineSound.emitEvent(soundResource.c_str());
	eventInstance->setUserData(new SoundData{ true });
	eventInstance->setCallback([](FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE *eventInstance, void *parameters) -> FMOD_RESULT {
		FMOD::Studio::EventInstance *instance = (FMOD::Studio::EventInstance *)eventInstance;
		SoundData* sounData = nullptr;
		instance->getUserData((void**)&sounData);
		if (sounData && sounData->releaseOnStop) {
			delete sounData;
		}
		return FMOD_OK;
	}, FMOD_STUDIO_EVENT_CALLBACK_SOUND_STOPPED);
	eventInstance->release();
}

void TCompSoundEmmiter::start() {
}

void TCompSoundEmmiter::stop() {

}

bool TCompSoundEmmiter::isEmmiting(const FMOD::Studio::EventInstance* eventInstance) const {
	if (!eventInstance || !eventInstance->isValid()) {
		return false;
	}
	FMOD_STUDIO_PLAYBACK_STATE playbackState;
	eventInstance->getPlaybackState(&playbackState);
	return playbackState != FMOD_STUDIO_PLAYBACK_STOPPED;
}

bool TCompSoundEmmiter::isEmmiting() {
	return !eventInstances.empty();
}

