#include "mcv_platform.h"
#include "comp_platform_move.h"
#include "render/render_utils.h"

DECL_OBJ_MANAGER("platform_move", TCompPlatformMove);

void TCompPlatformMove::debugInMenu() {
    if (ImGui::TreeNode("Waypoints")) {
        for (auto& v : waypoints) {
            ImGui::PushID(&v);
            ImGui::DragFloat3("Point", &v.x, 0.1f, -20.f, 20.f);
            ImGui::PopID();
        }
        ImGui::TreePop();
    }

    for (size_t i = 0; i < waypoints.size(); ++i)
        renderLine(waypoints[i],
                   waypoints[(i + 1) % waypoints.size()],
                   VEC4(0, 1, 0, 1));
}

void TCompPlatformMove::registerMsgs() {
	DECL_MSG(TCompPlatformMove, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompPlatformMove, TMsgTriggerEnter, onTriggerEnter);
	DECL_MSG(TCompPlatformMove, TMsgTriggerExit, onTriggerExit);
}

void TCompPlatformMove::load(const json& j, TEntityParseContext& ctx) {
    type = j.value("type", "NONE");
    speed = j.value("speed", 1.0f);

    auto& j_waypoints = j["waypoints"];
    for (auto it = j_waypoints.begin(); it != j_waypoints.end(); ++it) {
        VEC3 p = loadVEC3(it.value());
        waypoints.push_back(p);
    }
}

void TCompPlatformMove::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
    transform = get<TCompTransform>();

    player = (CEntity *)getEntityByName("The Player");
    playerTransform = player->get<TCompTransform>();
}

void TCompPlatformMove::onTriggerEnter(const TMsgTriggerEnter & msg) {
	CEntity *triggerer = msg.h_other_entity;
	std::string triggererName = triggerer->getName();
	dbg("Platform trigger enter by %s\n", triggererName.c_str());
	if (triggererName == "The Player") {
		
	}
}

void TCompPlatformMove::onTriggerExit(const TMsgTriggerExit & msg)
{
}

void TCompPlatformMove::update(float dt) {
    if (!transform) return;
    VEC3 myPosition = transform->getPosition();

    if (VEC3::Distance(myPosition, waypoints[currentWaypoint]) <= 0.2f) {
        currentWaypoint = (currentWaypoint + 1) % waypoints.size();
    }

    VEC3 direction = waypoints[currentWaypoint] - myPosition;
    direction.Normalize();
    VEC3 movement = direction * speed * dt;

    collider = get<TCompCollider>();
    assert(collider);
    PxRigidDynamic *rigidDynamic = (PxRigidDynamic*)collider->actor;
    rigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
    rigidDynamic->setKinematicTarget({ myPosition.x + movement.x,
                                       myPosition.y + movement.y,
                                       myPosition.z + movement.z });

	//// Parent is a trigger collider entity.
	//TCompHierarchy* parent = get<TCompHierarchy>();
	//TCompTransform* parentTransf = parent->h_parent_transform;
	//VEC3 parentPos = parentTransf->getPosition();
	//TCompCollider* parentCollider = parent->get<TCompCollider>();
	//assert(parentCollider);
	//PxRigidDynamic* parentRigidDynamic = (PxRigidDynamic*)parentCollider->actor;
	//parentRigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	//rigidDynamic->setKinematicTarget({ parentPos.x + movement.x,
	//								   parentPos.y + movement.y,
	//								   parentPos.z + movement.z });


    // TO DO: The player should move with the platform.
    if ( false /*player touches platform*/ ) {
        // ¿Hacer al player hijo de la plataforma mientras esté sobre la plataforma?
        VEC3 playerPos = playerTransform->getPosition();
        playerTransform->setPosition(playerPos + movement);
    }
}
