#pragma once

#include "comp_base.h"
#include "entity/common_msgs.h"
#include "entity/entity_parser.h"
#include "components/comp_transform.h"
#include "PxPhysicsAPI.h"

class TCompPlatformMove: public TCompBase {
	DECL_SIBLING_ACCESS();

    std::string         type = "";
    std::vector<VEC3>   waypoints;
    size_t              currentWaypoint = 0;
    float               speed = 0.f;

    TCompTransform*     transform;
    CEntity*            player;
    TCompTransform*     playerTransform;
    TCompCollider*      collider;

public:
    void debugInMenu();
    static void registerMsgs();
    void load(const json& j, TEntityParseContext& ctx);

    void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
    void update(float dt);

};
