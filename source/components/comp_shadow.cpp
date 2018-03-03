#include "mcv_platform.h"
#include "comp_shadow.h"
#include "comp_transform.h"
#include "comp_hierarchy.h"

DECL_OBJ_MANAGER("shadow", TCompShadow);

void TCompShadow::debugInMenu() {
}

void TCompShadow::load(const json& j, TEntityParseContext& ctx) {
}

void TCompShadow::update(float dt) {
	TCompHierarchy *hierarchy = get<TCompHierarchy>();
	TCompTransform *parentTransform = hierarchy->h_parent_transform;

	PxScene* scene = Engine.getPhysics().getScene();
	PxVec3 origin = { parentTransform->getPosition().x, parentTransform->getPosition().y + 0.1f, parentTransform->getPosition().z };
	PxVec3 unitDir = { 0,-1, 0};
	PxReal maxDistance = 1000;
	PxRaycastBuffer hit;
	bool status = scene->raycast(origin, unitDir, maxDistance, hit);

	if (status) {
		TCompTransform *transform = get<TCompTransform>();
		transform->setPosition({ hit.block.position.x, hit.block.position.y, hit.block.position.z });
	}
}