#pragma once

#include "modules/module.h"

class CModuleMainMenu : public IModule {
private:
	bool imguiBool = true;

public:
	CModuleMainMenu(const std::string& name) : IModule(name) {}
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;
};