#pragma once

#include "modules/module.h"

class CModuleSound : public IModule {
private:
	struct FollowingEvent {
		FMOD::Studio::EventInstance* eventInstance = nullptr;
		CHandle transformHandle;
	};
	FMOD_RESULT res;
	void *extraDriverData = nullptr;
	FMOD::System* lowLevelSystem = nullptr;
	FMOD::Studio::System* system = nullptr;

	std::map<std::string, FMOD::Studio::Bank*> banks;

	FMOD_3D_ATTRIBUTES listenerAttributes;
	CHandle cameraHandle;

	std::vector<FollowingEvent> followingEvents;

	std::set<std::string> unpausableEvents;

	std::string soundsVCA;
	std::string musicVCA;


	float masterVolume = 1.f;
	float soundVolume = 1.f;
	float musicVolume = 1.f;

	bool paused = false;

	void updateListenerAttributes();
	void updateFollowingEvents();

public:
	CModuleSound(const std::string& name);
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;

	FMOD::Studio::EventInstance* emitFollowingEvent(const std::string& sound, CHandle transformHandle);
	FMOD::Studio::EventInstance* emitEvent(const std::string& sound, const CTransform* transform);
	FMOD::Studio::EventInstance* emitEvent(const std::string& sound, const FMOD_3D_ATTRIBUTES* attributes = nullptr);
	FMOD::Studio::EventInstance* getEventInstance(const std::string& sound, const CTransform* transform);
	FMOD::Studio::EventInstance* getEventInstance(const std::string& sound, const FMOD_3D_ATTRIBUTES* attributes = nullptr);
	FMOD::Studio::EventDescription* getEventDescription(const std::string& sound);
	FMOD::Studio::EventInstance* emitEventFromDescriptor(FMOD::Studio::EventDescription* descriptor, const CTransform* transform);
	FMOD::Studio::EventInstance* emitEventFromDescriptor(FMOD::Studio::EventDescription* descriptor, FMOD_3D_ATTRIBUTES* attributes = nullptr);
	FMOD::Studio::System* getSystem();
	void stopEvent(FMOD::Studio::EventInstance * instance, bool fadeout = false);

	void setMasterVolume(float volume);
	void setSoundVolume(float volume);
	void setMusicVolume(float volume);

	void setPaused(bool paused);

	void forEachEventInstance(std::function<void(FMOD::Studio::Bank*, FMOD::Studio::EventDescription*, FMOD::Studio::EventInstance*)> callback);
	void forEachEventInstance(FMOD::Studio::Bank * bank, std::function<void(FMOD::Studio::EventDescription*, FMOD::Studio::EventInstance*)> callback);
};


