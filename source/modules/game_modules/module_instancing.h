#pragma once

#include "modules/module.h"
#include "render/mesh/mesh_instanced.h"

class CModuleInstancing : public IModule {
private:

	
	struct TInstance {
		MAT44 world;
	};

	struct TInstanceBlood {
		MAT44 world;
		VEC4  color;
	};
	
	struct TRenderParticle {
		VEC3  pos;      // Maps to iCenter.xyz
		float angle;    // Maps to iCenter.w
		VEC4  color;
		float scale_x;
		float scale_y;
		float nframe;
	};
	
	struct TGrassParticle {
		VEC3  pos;
	};

	struct InstanceDataAABB {
		TInstance instanceData;
		AABB aabb;
	};

	std::map<std::string, std::pair<std::vector<TInstance>, std::vector<AABB>>> instancesDataMap;

	bool culling = true;

	void loadInstances(const json & jInstances);
public:
	CModuleInstancing(const std::string& name)
		: IModule(name) {
	}
	bool start() override;
	void update(float delta) override;
	void render() override;
	void setAllInstancesData();
};
