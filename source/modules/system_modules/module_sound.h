#pragma once

#include "modules/module.h"

class CModuleSound : public IModule {
	struct ListenerAttributes {
		FMOD_VECTOR position;
		FMOD_VECTOR velocity;
		FMOD_VECTOR forward;
		FMOD_VECTOR up;
	};
private:
	FMOD_RESULT res;
	void *extraDriverData = nullptr;
	FMOD::System* lowLevelSystem = nullptr;
	FMOD::Studio::System* system = nullptr;

	std::map<std::string, FMOD::Studio::Bank*> banks;
	std::map<std::string, FMOD::Studio::EventInstance*> eventInstances;
	ListenerAttributes listenerAttributes;

public:
	CModuleSound(const std::string& name);
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void updateListenerAttributes(VEC3& position, VEC3& speed, VEC3& forwrad, VEC3& up);
	FMOD::Studio::EventInstance* instanceEvent(const char* event);
	void releaseEvent(const char* event);

	void startEvent(const char* event, const FMOD_3D_ATTRIBUTES* attributes = nullptr);
	void stopEvent(const char* event, FMOD_STUDIO_STOP_MODE mode = FMOD_STUDIO_STOP_IMMEDIATE);

	void emitEvent(const char* event, const FMOD_3D_ATTRIBUTES* attributes = nullptr);

};


