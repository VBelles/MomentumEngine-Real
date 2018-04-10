#include "mcv_platform.h"
#include "comp_shadow.h"
#include "comp_transform.h"
#include "comp_hierarchy.h"

#include "render/render_objects.h"
#include "render/texture/texture.h"
#include "render/texture/material.h"

DECL_OBJ_MANAGER("shadow", TCompShadow);


void TCompShadow::load(const json& j, TEntityParseContext& ctx) {
	if (j.count("offset")) {
		offset = loadVEC3(j["offset"]);
	}

	shadowMeshName = j.value("mesh", "axis.mesh");
	mesh = Resources.get(shadowMeshName)->as<CRenderMesh>();

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
	if (enabled) {
		TCompTransform* parentTransform = parentTransformHandle;
		TCompTransform* transform = transformHandle;

		PxScene* scene = Engine.getPhysics().getScene();
		PxVec3 origin = { parentTransform->getPosition().x + offset.x, parentTransform->getPosition().y + offset.y, parentTransform->getPosition().z + offset.z };

		const PxU32 bufferSize = 256;
		PxRaycastHit hitBuffer[bufferSize];
		PxRaycastBuffer buf(hitBuffer, bufferSize);

		bool status = scene->raycast(origin, unitDir, maxDistance, buf);
		PxRaycastHit nearest = buf.touches[0];
		nearest.distance = maxDistance + 1;
		for (PxU32 i = 0; i < buf.nbTouches; i++) {
			if (!buf.touches[i].shape->getFlags().isSet(PxShapeFlag::eTRIGGER_SHAPE)) {
				if (buf.touches[i].distance < nearest.distance) {
					nearest = buf.touches[i];
				}
			}
		}
		//Sumamos 0.001 a position.y para evitar z fighting
		transform->setPosition({ nearest.position.x, nearest.position.y + 0.001f, nearest.position.z });
	}
}


void TCompShadow::setMesh(std::string meshName) {
	mesh = Resources.get(meshName)->as<CRenderMesh>();
}

void TCompShadow::enable() {
	if (!enabled) {
		setMesh(shadowMeshName);
		enabled = true;
	}
}

void TCompShadow::disable() {
	if (enabled) {
		setMesh(disabledMeshName);
		enabled = false;
	}
}