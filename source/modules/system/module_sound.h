#pragma once

#include "modules/module.h"

class CModuleSound : public IModule {
private:
	void *extraDriverData = nullptr;
	FMOD::System* lowLevelSystem = nullptr;
	FMOD::Studio::System* system = nullptr;

	std::map<std::string, FMOD::Studio::Bank*> banks;

public:
	CModuleSound(const std::string& name);
	bool start() override;
	bool stop() override;
	void update(float delta) override;

};


