#pragma once
#include "modules/module.h"

class CModuleSlash : public IModule {
public:
	CModuleSlash(const std::string& name);
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;

};

