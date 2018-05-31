#include "mcv_platform.h"
#include "modules/system_modules/instancing/module_instancing.h"
#include "modules/system_modules/instancing/Instances.h"
#include "entity/entity_parser.h"
#include "render/mesh/mesh_instanced.h"

CModuleInstancing::CModuleInstancing(const std::string& name) : IModule(name) {
}

bool CModuleInstancing::start() {
	//TEntityParseContext ctx;
	//parseScene("data/instances/instances.json", ctx);

	auto rmesh = Resources.get("data/instances/grass.instanced_mesh")->as<CRenderMesh>();
	instanceMeshes[Grass] = (CRenderMeshInstanced*)rmesh;

	return true;
}

bool CModuleInstancing::stop() {
	return true;
}

void CModuleInstancing::update(float delta) {
}

void CModuleInstancing::addInstance(InstanceType type, VEC3 pos) {
	if (type == Grass) {
		TGrassParticle new_instance;
		new_instance.pos = VEC3(-5, 9, -49);
		instances[Grass].push_back(new_instance);
		instanceMeshes[Grass]->setInstancesData(instances[Grass].data(), instances[Grass].size(), sizeof(TGrassParticle));
	}
}