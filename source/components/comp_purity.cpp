#include "mcv_platform.h"
#include "comp_purity.h"
#include "render/render_utils.h"
#include "player/comp_player_model.h"
#include "PxPhysicsAPI.h"

DECL_OBJ_MANAGER("purity", TCompPurity);

void TCompPurity::debugInMenu() {
}

void TCompPurity::registerMsgs() {
	DECL_MSG(TCompPurity, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompPurity, TMsgPowerLvlChange, onPowerLvlChange);
}

void TCompPurity::load(const json& j, TEntityParseContext& ctx) {
}

void TCompPurity::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
    player = (CHandle)getEntityByName("The Player");
}

// Pruebo en WWBox061, la tercera plataforma fija que hay al principio.
void TCompPurity::onPowerLvlChange(const TMsgPowerLvlChange & msg) {
    TCompCollider* collider = get<TCompCollider>();
    assert(collider);
	TCompRender *render = get<TCompRender>();
	assert(render);
 
    if (msg.powerLvl == 1) {
		collider->enable();
		//render->setMesh(originalMeshPath, originalMaterialPath);
    }
    else {
		collider->disable();
		//render->setMesh("data/meshes/.mesh");
    }
}

void TCompPurity::update(float dt) {
}
