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
    PxRigidDynamic* rigidDynamic = (PxRigidDynamic*)collider->actor;
    
    const PxU32 numShapes = rigidDynamic->getNbShapes();
    PxShape** shapes = (PxShape**)std::malloc(sizeof(PxShape*)*numShapes);
    rigidDynamic->getShapes(shapes, numShapes);
 
    if (msg.powerLvl == 1) {
        for (PxU32 i = 0; i < numShapes; ++i) {
            auto f = shapes[i]->getFlags();
            shapes[i]->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
            shapes[i]->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
        }
        rigidDynamic->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
    }
    else {
        rigidDynamic->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
        for (PxU32 i = 0; i < numShapes; ++i) {
            auto f = shapes[i]->getFlags();
            shapes[i]->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
            shapes[i]->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
        }
    }

    std::free(shapes);
}

void TCompPurity::update(float dt) {
}
