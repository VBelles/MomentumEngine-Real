#include "mcv_platform.h"
#include "comp_purity.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"
#include "render/render_utils.h"
#include "render/texture/material.h"
#include "components/comp_render.h"
#include "components/comp_collider.h"
#include "components/comp_transform.h"

DECL_OBJ_MANAGER("purity", TCompPurity);


void TCompPurity::registerMsgs() {
	DECL_MSG(TCompPurity, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompPurity, TMsgPurityChange, onPurityChange);
}

void TCompPurity::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	renderHandle = get<TCompRender>();
	assert(renderHandle.isValid());
	colliderHandle = get<TCompCollider>();
	assert(colliderHandle.isValid());
	assert(getRigidDynamic());
	originalMeshPath = getRender()->meshes[0].mesh->getName();
	originalMaterialPath = getRender()->meshes[0].materials[0]->getName();
	getRigidDynamic()->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
}

// Pruebo en WWBox061, la tercera plataforma fija que hay al principio.
void TCompPurity::onPurityChange(const TMsgPurityChange& msg) {
	TCompCollider* collider = getCollider();
	if (msg.isPure) {
		//collider->create();
		collider->setupFiltering(collider->config.group, collider->config.mask | EnginePhysics.Player);
		getRender()->setMesh(originalMeshPath, originalMaterialPath);
	}
	else {
		//collider->destroy();
		collider->setupFiltering(collider->config.group, collider->config.mask & !EnginePhysics.Player);
		// Falta decidir qué efecto aplicar cuando la plataforma está intangible,
		// de momento la dejo con material blanco.
		getRender()->setMesh(originalMeshPath, "data/materials/white.material");
	}
}


TCompRender* TCompPurity::getRender() {
	return renderHandle;
}

TCompCollider* TCompPurity::getCollider() {
	return colliderHandle;
}

PxRigidDynamic* TCompPurity::getRigidDynamic() {
	return static_cast<PxRigidDynamic*>(getCollider()->actor);
}


