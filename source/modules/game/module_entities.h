#pragma once

#include "modules/module.h"
#include "entity/entity.h"

class CModuleEntities : public IModule {
	std::vector< CHandleManager* > om_to_update;
	std::vector< CHandleManager* > om_to_render_debug;
	void loadListOfManagers(const json& j, std::vector< CHandleManager* > &managers);
	void renderDebugOfComponents();

public:
	float time_scale_factor = 1.f;
	CModuleEntities(const std::string& aname) : IModule(aname) {}
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;

	void reset();
};

CHandle getEntityByName(const std::string& name);
