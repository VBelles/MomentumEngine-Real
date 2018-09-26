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
	FMOD::Studio::System* getSystem();
	void stopEvent(FMOD::Studio::EventInstance * instance, bool fadeout = false);


};


