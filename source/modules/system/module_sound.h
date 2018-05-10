#pragma once

#include "modules/module.h"

class CModuleSound : public IModule {
private:

public:
	CModuleSound(const std::string& name);
	bool start() override;
	bool stop() override;
	void update(float delta) override;

};


