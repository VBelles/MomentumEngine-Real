#pragma once

#include "modules/module.h"
#include "navmesh/navmesh.h"
#include "navmesh/navmesh_query.h"

class CModuleNavmesh : public IModule {
private:
	CNavMesh navmesh;
	CNavMeshQuery navQuery{&navmesh};

public:
	CModuleNavmesh(const std::string& name);
	bool start() override;
	void render() override;
	bool stop() override;
	void update(float delta) override;
};
