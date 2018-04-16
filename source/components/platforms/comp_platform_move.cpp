#include "mcv_platform.h"
#include "comp_platform_move.h"
#include "render/render_objects.h"
#include "components/player/comp_player_model.h"

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
    player = (CHandle)getEntityByName(PLAYER_NAME);
	TCompCollider* collider = get<TCompCollider>();
	assert(collider);
	PxRigidDynamic *rigidDynamic = (PxRigidDynamic*)collider->actor;
	rigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
}

void TCompPlatformMove::onTriggerEnter(const TMsgTriggerEnter & msg) {
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

void TCompPlatformMove::onTriggerExit(const TMsgTriggerExit & msg) {
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

void TCompPlatformMove::update(float dt) {
    TCompTransform* transform = get<TCompTransform>();
    if (!transform) return;
    VEC3 myPosition = transform->getPosition();

    if (VEC3::Distance(myPosition, waypoints[currentWaypoint]) <= 0.2f) {
        currentWaypoint = (currentWaypoint + 1) % waypoints.size();
    }

    VEC3 direction = waypoints[currentWaypoint] - myPosition;
    direction.Normalize();

	VEC3 movement = direction * speed * dt;

    TCompCollider* collider = get<TCompCollider>();
    assert(collider);
    PxRigidDynamic* rigidDynamic = (PxRigidDynamic*)collider->actor;

	PxTransform newTransform;
	newTransform.p = {  myPosition.x + movement.x,
                        myPosition.y + movement.y,
                        myPosition.z + movement.z };
	newTransform.q = {  transform->getRotation().x,
                        transform->getRotation().y,
                        transform->getRotation().z,
                        transform->getRotation().w };
	rigidDynamic->setKinematicTarget(newTransform);

	transform->setPosition({ newTransform.p.x, newTransform.p.y, newTransform.p.z });

    // Move the player with the platform.
    if ( isPlayerInTrigger ) {
        CEntity* ePlayer = (CEntity*)player;
		TCompTransform* playerTransform = ePlayer->get<TCompTransform>();
		TCompPlayerModel* playerModel = ePlayer->get<TCompPlayerModel>();
		playerModel->getCollider()->controller->setFootPosition({
			playerTransform->getPosition().x + movement.x,
			playerTransform->getPosition().y + movement.y,
			playerTransform->getPosition().z + movement.z
			});
    }
}
