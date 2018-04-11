#include "mcv_platform.h"
#include "comp_platform.h"
#include "render/render_utils.h"
#include "player/comp_player_model.h"
#include "controllers/comp_follow_curve.h"
#include "geometry/curve.h"

DECL_OBJ_MANAGER("platform", TCompPlatform);

void TCompPlatform::debugInMenu() {
}

void TCompPlatform::registerMsgs() {
	DECL_MSG(TCompPlatform, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompPlatform, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompPlatform, TMsgTriggerExit, onTriggerExit);
}

void TCompPlatform::load(const json& j, TEntityParseContext& ctx) {
}

void TCompPlatform::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
    player = (CHandle)getEntityByName(PLAYER_NAME);
	TCompCollider* collider = get<TCompCollider>();
	assert(collider);
	PxRigidDynamic *rigidDynamic = (PxRigidDynamic*)collider->actor;
	rigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
}

void TCompPlatform::onTriggerEnter(const TMsgTriggerEnter & msg) {
	CEntity *triggerer = msg.h_other_entity;
	std::string triggererName = triggerer->getName();
	//dbg("Platform trigger enter by %s\n", triggererName.c_str());
	if (triggererName == PLAYER_NAME) {
        isPlayerInTrigger = true;

		CEntity* ePlayer = (CEntity*)player;
		TCompPlayerModel* playerModel = ePlayer->get<TCompPlayerModel>();
		playerModel->isAttachedToPlatform = true;
	}
}

void TCompPlatform::onTriggerExit(const TMsgTriggerExit & msg) {
    CEntity *triggerer = msg.h_other_entity;
    std::string triggererName = triggerer->getName();
    //dbg("Platform trigger exit by %s\n", triggererName.c_str());
    if (triggererName == PLAYER_NAME) {
        isPlayerInTrigger = false;
		CEntity* ePlayer = (CEntity*)player;
		TCompPlayerModel* playerModel = ePlayer->get<TCompPlayerModel>();
		playerModel->isAttachedToPlatform = false;
    }
}

void TCompPlatform::update(float dt) {
    TCompTransform* transform = get<TCompTransform>();
    if (!transform) return;
    VEC3 myPosition = transform->getPosition();

    TCompCollider* collider = get<TCompCollider>();
    assert(collider);
    PxRigidDynamic* rigidDynamic = (PxRigidDynamic*)collider->actor;

    // Get comp_follow_curve movement.
    TCompHierarchy* hierarchy = get<TCompHierarchy>();
    CEntity* parent = hierarchy->h_parent;
    if (parent) {
        TCompFollowCurve* curveComp = parent->get<TCompFollowCurve>();
        VEC3 movement = curveComp->getMovement();

        // Move the player with the platform.
        if ( isPlayerInTrigger ) {
            CEntity* ePlayer = (CEntity*)player;
		    TCompTransform* playerTransform = ePlayer->get<TCompTransform>();
		    TCompPlayerModel* playerModel = ePlayer->get<TCompPlayerModel>();
		    playerModel->GetCollider()->controller->setFootPosition({
			    playerTransform->getPosition().x + movement.x,
			    playerTransform->getPosition().y + movement.y,
			    playerTransform->getPosition().z + movement.z });
        }
    }
}
