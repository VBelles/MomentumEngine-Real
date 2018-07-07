#pragma once

#include "modules/module.h"
#include "navmesh/navmesh.h"
#include "navmesh/navmesh_query.h"

class CModuleNavmesh : public IModule {
private:
	std::map<std::string, std::pair<CNavMesh*, CNavMeshQuery*>> navMeshes;
	std::map<std::string, bool> renderNavMesh;

public:
	CModuleNavmesh(const std::string& name);
	bool start() override;
	void render() override;
	bool stop() override;
	void update(float delta) override;

	CNavMesh* getNavMesh(std::string id);
	CNavMeshQuery* getNavQuery(std::string id);
};
