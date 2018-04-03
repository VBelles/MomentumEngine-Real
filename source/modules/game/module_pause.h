#pragma once

#include "modules/module.h"
#include "geometry/curve.h"

class CModulePause : public IModule {
private:
	bool pause = false;

public:
	CModulePause(const std::string& aname) : IModule(aname) {}
    bool start() override;
    bool stop() override;
    void update(float delta) override;
    void render() override;
};