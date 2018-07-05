#pragma once

#include "modules/module.h"
#include "render/mesh/mesh_instanced.h"

class CModuleTestInstancing : public IModule {
private:
	// -------------------------------------------------------------------
	struct TInstance {
		MAT44 world;
	};
	CRenderMeshInstanced* instances_mesh = nullptr;
	std::vector< TInstance > instances;

	CRenderMeshInstanced* instances_mesh_grass_small_00 = nullptr;
	std::vector< TInstance > instances_grass_small_00;
	CRenderMeshInstanced* instances_mesh_grass_small_01 = nullptr;
	std::vector< TInstance > instances_grass_small_01;
	CRenderMeshInstanced* instances_mesh_grass_small_02 = nullptr;
	std::vector< TInstance > instances_grass_small_02;
	CRenderMeshInstanced* instances_mesh_grass_medium_00 = nullptr;
	std::vector< TInstance > instances_grass_medium_00;
	CRenderMeshInstanced* instances_mesh_grass_medium_01 = nullptr;
	std::vector< TInstance > instances_grass_medium_01;
	CRenderMeshInstanced* instances_mesh_grass_medium_02 = nullptr;
	std::vector< TInstance > instances_grass_medium_02;
	CRenderMeshInstanced* instances_mesh_grass_large_00 = nullptr;
	std::vector< TInstance > instances_grass_large_00;
	CRenderMeshInstanced* instances_mesh_grass_large_01 = nullptr;
	std::vector< TInstance > instances_grass_large_01;
	CRenderMeshInstanced* instances_mesh_grass_large_02 = nullptr;
	std::vector< TInstance > instances_grass_large_02;
	CRenderMeshInstanced* instances_mesh_moss_small_00 = nullptr;
	std::vector< TInstance > instances_moss_small_00;
	CRenderMeshInstanced* instances_mesh_moss_small_01 = nullptr;
	std::vector< TInstance > instances_moss_small_01;
	CRenderMeshInstanced* instances_mesh_moss_small_02 = nullptr;
	std::vector< TInstance > instances_moss_small_02;
	CRenderMeshInstanced* instances_mesh_moss_small_03 = nullptr;
	std::vector< TInstance > instances_moss_small_03;
	CRenderMeshInstanced* instances_mesh_moss_small_04 = nullptr;
	std::vector< TInstance > instances_moss_small_04;
	CRenderMeshInstanced* instances_mesh_moss_small_05 = nullptr;
	std::vector< TInstance > instances_moss_small_05;
	CRenderMeshInstanced* instances_mesh_moss_small_06 = nullptr;
	std::vector< TInstance > instances_moss_small_06;
	CRenderMeshInstanced* instances_mesh_moss_medium_00 = nullptr;
	std::vector< TInstance > instances_moss_medium_00;
	CRenderMeshInstanced* instances_mesh_moss_medium_01 = nullptr;
	std::vector< TInstance > instances_moss_medium_01;
	CRenderMeshInstanced* instances_mesh_moss_medium_02 = nullptr;
	std::vector< TInstance > instances_moss_medium_02;
	CRenderMeshInstanced* instances_mesh_moss_large = nullptr;
	std::vector< TInstance > instances_moss_large;
	CRenderMeshInstanced* instances_mesh_moss_extra_large = nullptr;
	std::vector< TInstance > instances_moss_extra_large;
	CRenderMeshInstanced* instances_mesh_petal_00 = nullptr;
	std::vector< TInstance > instances_petal_00;
	CRenderMeshInstanced* instances_mesh_petal_01 = nullptr;
	std::vector< TInstance > instances_petal_01;
	CRenderMeshInstanced* instances_mesh_leaf_00 = nullptr;
	std::vector< TInstance > instances_leaf_00;
	CRenderMeshInstanced* instances_mesh_leaf_01 = nullptr;
	std::vector< TInstance > instances_leaf_01;
	CRenderMeshInstanced* instances_mesh_leaf_02 = nullptr;
	std::vector< TInstance > instances_leaf_02;
	CRenderMeshInstanced* instances_mesh_leaf_03 = nullptr;
	std::vector< TInstance > instances_leaf_03;
	CRenderMeshInstanced* instances_mesh_bush = nullptr;
	std::vector< TInstance > instances_bush;

	// -------------------------------------------------------------------
	struct TInstanceBlood {
		MAT44 world;
		VEC4  color;
	};
	CRenderMeshInstanced* blood_instances_mesh = nullptr;
	std::vector< TInstanceBlood > blood_instances;

	// -------------------------------------------------------------------
	struct TRenderParticle {
		VEC3  pos;      // Maps to iCenter.xyz
		float angle;    // Maps to iCenter.w
		VEC4  color;
		float scale_x;
		float scale_y;
		float nframe;
	};
	CRenderMeshInstanced* particles_instances_mesh = nullptr;
	std::vector< TRenderParticle > particles_instances;

	// -------------------------------------------------------------------
	struct TGrassParticle {
		VEC3  pos;
	};
	CRenderMeshInstanced* grass_instances_mesh = nullptr;
	std::vector< TGrassParticle > grass_instances;

	int nGrassToInstance = 0;
	float sz = 50.f;

public:
	CModuleTestInstancing(const std::string& name)
		: IModule(name) {
	}
	bool start() override;
	void update(float delta) override;
	void render() override;
};