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

    // TO DO: The player should move with the platform.

}
