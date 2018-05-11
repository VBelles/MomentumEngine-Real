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

	void instanceEvent(const char * event);
	void releaseEvent(const char * event);

	void startEvent(const char * event);
	void stopEvent(const char * event, FMOD_STUDIO_STOP_MODE mode = FMOD_STUDIO_STOP_IMMEDIATE);

	void emitEvent(const char * event);

};


