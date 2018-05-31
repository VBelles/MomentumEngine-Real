#pragma once

#include "modules/module.h"

struct IInstance;
class CRenderMeshInstanced;

enum InstanceType {
	Grass
};

class CModuleInstancing : public IModule {
private:
	struct TGrassParticle {
		VEC3  pos;
	};
	std::unordered_map<InstanceType, std::vector<TGrassParticle>> instances;
	std::unordered_map<InstanceType, CRenderMeshInstanced*> instanceMeshes;


public:
	CModuleInstancing(const std::string& name);
	bool start() override;
	bool stop() override;
	void update(float delta) override;

	void addInstance(InstanceType type, VEC3 pos);
};
