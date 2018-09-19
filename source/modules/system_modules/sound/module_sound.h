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
	FMOD::Studio::EventInstance* emitEvent(const char* sound, const CTransform& transform);
	FMOD::Studio::EventInstance* emitEvent(const char* sound, const FMOD_3D_ATTRIBUTES* attributes = nullptr);

	FMOD::Studio::System* getSystem();

};


