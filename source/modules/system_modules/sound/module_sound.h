#pragma once

#include "modules/module.h"

class CModuleSound : public IModule {
private:
	FMOD_RESULT res;
	void *extraDriverData = nullptr;
	FMOD::System* lowLevelSystem = nullptr;
	FMOD::Studio::System* system = nullptr;

	std::map<std::string, FMOD::Studio::Bank*> banks;
	std::map<std::string, FMOD::Studio::EventInstance*> eventInstances;
	FMOD_3D_ATTRIBUTES listenerAttributes;

	CHandle cameraHandle;

	void updateListenerAttributes();

public:
	CModuleSound(const std::string& name);
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	FMOD::Studio::EventInstance* instanceEvent(const char* sound);
	void releaseEvent(const char* sound);


	void startEvent(const char* sound, const FMOD_3D_ATTRIBUTES* attributes = nullptr);
	void stopEvent(const char* sound, FMOD_STUDIO_STOP_MODE mode = FMOD_STUDIO_STOP_IMMEDIATE);

	FMOD::Studio::EventInstance* emitEvent(const char* sound, VEC3 position, VEC3 velocity = VEC3::Zero, VEC3 forward = VEC3::Zero, VEC3 up = VEC3::Zero);
	FMOD::Studio::EventInstance* emitEvent(const char* sound, const FMOD_3D_ATTRIBUTES* attributes = nullptr);

	FMOD::Studio::System* getSystem();

};


