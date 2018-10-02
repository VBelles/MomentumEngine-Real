#include "mcv_platform.h"
#include "comp_purity.h"
#include "entity/entity_parser.h"
#include "entity/common_msgs.h"
#include "render/render_utils.h"
#include "render/texture/material.h"

DECL_OBJ_MANAGER("purity", TCompPurity);

void TCompPurity::registerMsgs() {
	DECL_MSG(TCompPurity, TMsgEntityCreated, onCreated);
	DECL_MSG(TCompPurity, TMsgPurityChange, onPurityChange);
}

void TCompPurity::load(const json& j, TEntityParseContext& ctx) {
}

void TCompPurity::onCreated(const TMsgEntityCreated & msg) {
	renderHandle = get<TCompRender>();
	assert(renderHandle.isValid());
	colliderHandle = get<TCompCollider>();
	assert(colliderHandle.isValid());
	getRender()->meshes[0].enabled = true;
	getRender()->refreshMeshesInRenderManager();
}

void TCompPurity::onPurityChange(const TMsgPurityChange& msg) {
	TCompCollider* collider = getCollider();
	TCompRender* render = getRender();
	if (msg.isPure) { //Player can stand on
		collider->setupFiltering(collider->config.group, collider->config.mask | EnginePhysics.Player);
		render->meshes[0].enabled = true;
		render->meshes[1].enabled = false;
	}
	else { //Player can NOT stand on
		collider->setupFiltering(collider->config.group, collider->config.mask & !EnginePhysics.Player);
		render->meshes[0].enabled = false;
		render->meshes[1].enabled = true;
	}
	render->refreshMeshesInRenderManager();
}


TCompRender* TCompPurity::getRender() {
	return renderHandle;
}

TCompCollider* TCompPurity::getCollider() {
	return colliderHandle;
}



