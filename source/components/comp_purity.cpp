#include "mcv_platform.h"
#include "comp_purity.h"
#include "render/render_utils.h"
#include "player/comp_player_model.h"
#include "entity/entity_parser.h"
#include "render/texture/material.h"
#include "PxPhysicsAPI.h"

DECL_OBJ_MANAGER("purity", TCompPurity);

void TCompPurity::registerMsgs() {
	DECL_MSG(TCompPurity, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompPurity, TMsgPurityChange, onPurityChange);
}

void TCompPurity::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	TCompRender* render = get<TCompRender>();
	originalMeshPath = render->meshes[0].mesh->getName();
	originalMaterialPath = render->meshes[0].materials[0]->getName();
}

// Pruebo en WWBox061, la tercera plataforma fija que hay al principio.
void TCompPurity::onPurityChange(const TMsgPurityChange& msg) {
	TCompCollider* collider = get<TCompCollider>();
	assert(collider);

	TCompRender *render = get<TCompRender>();
	assert(render);

	if (msg.isPure) {
		collider->setupFiltering(collider->config.group, collider->config.mask | EnginePhysics.Player);
		render->setMesh(originalMeshPath, originalMaterialPath);
	}
	else {
		collider->setupFiltering(collider->config.group, collider->config.mask & !EnginePhysics.Player);
		// Falta decidir qué efecto aplicar cuando la plataforma está intangible,
		// de momento la dejo con material blanco.
		render->setMesh(originalMeshPath, "data/materials/white.material");
	}
}



