#include "mcv_platform.h"
#include "comp_shadow.h"
#include "comp_transform.h"
#include "comp_hierarchy.h"

#include "render/render_objects.h"
#include "render/texture/texture.h"
#include "render/texture/material.h"

DECL_OBJ_MANAGER("shadow", TCompShadow);

void TCompShadow::debugInMenu() {
}

void TCompShadow::load(const json& j, TEntityParseContext& ctx) {
	std::string name_mesh = j.value("mesh", "axis.mesh");
	mesh = Resources.get(name_mesh)->as<CRenderMesh>();

	if (j.count("materials")) {
		auto& j_mats = j["materials"];
		assert(j_mats.is_array());
		for (size_t i = 0; i < j_mats.size(); ++i) {
			std::string name_material = j_mats[i];
			const CMaterial* material = Resources.get(name_material)->as<CMaterial>();
			materials.push_back(material);
		}
	}
	else {
		const CMaterial* material = Resources.get("data/materials/solid.material")->as<CMaterial>();
		materials.push_back(material);
	}
}

void TCompShadow::registerMsgs() {
	DECL_MSG(TCompShadow, TMsgEntityCreated, onCreate);
	DECL_MSG(TCompShadow, TMsgEntitiesGroupCreated, onGroupCreated);
}

void TCompShadow::onCreate(const TMsgEntityCreated& msg) {
	auto om_transform = getObjectManager<TCompTransform>();
	transformHandle = om_transform->createHandle();
}

void TCompShadow::onGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	CEntity *parent = CHandle(this).getOwner();
	parentTransformHandle = parent->get<TCompTransform>();
}

void TCompShadow::update(float dt) {
	TCompTransform *parentTransform = parentTransformHandle;
	TCompTransform *transform = transformHandle;

	PxScene* scene = Engine.getPhysics().getScene();
	PxVec3 origin = { parentTransform->getPosition().x, parentTransform->getPosition().y + 0.1f, parentTransform->getPosition().z };

	PxRaycastBuffer hit;
	bool status = scene->raycast(origin, unitDir, maxDistance, hit);

	if (status) {
		transform->setPosition({ hit.block.position.x, hit.block.position.y, hit.block.position.z });
	}
}

TCompTransform* TCompShadow::getTransform() {
	return transformHandle;
}

void TCompShadow::setMesh(std::string meshName) {
	mesh = Resources.get(meshName)->as<CRenderMesh>();
}