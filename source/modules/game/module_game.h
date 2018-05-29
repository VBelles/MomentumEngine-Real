#pragma once

#include "gui/controllers/hud_controller.h"
#include "navmesh/navmesh.h"

class CModuleGame : public IModule {
private:
	//GUI::CHudController* hudController;
    const CNavMesh* navmesh = nullptr;
public:
	CModuleGame(const std::string& aname) : IModule(aname) {}
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;
};
