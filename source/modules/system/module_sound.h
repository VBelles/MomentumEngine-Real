#pragma once

#include "modules/module.h"

#define TEST_EVENT "event:/onTest"

class CModuleSound : public IModule {
private:
	FMOD_RESULT res;
	void *extraDriverData = nullptr;
	FMOD::System* lowLevelSystem = nullptr;
	FMOD::Studio::System* system = nullptr;

	std::map<std::string, FMOD::Studio::Bank*> banks;
	std::map<std::string, FMOD::Studio::EventInstance*> eventInstances;

public:
	CModuleSound(const std::string& name);
	bool start() override;
	bool stop() override;
	void update(float delta) override;

	void instanceEvent(std::string event);
	void releaseEvent(std::string event);

	void startEvent(std::string event);
	void stopEvent(std::string event, FMOD_STUDIO_STOP_MODE mode = FMOD_STUDIO_STOP_IMMEDIATE);

	void emitEvent(std::string event);

};


