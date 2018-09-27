#pragma once

#include "entity/common_msgs.h"

class CCurve;

class TCompFollowCurve : public TCompBase {
    DECL_SIBLING_ACCESS();

    CHandle transformHandle;
    CHandle colliderHandle;

    std::unique_ptr<CCurve> curve = std::make_unique<CCurve>();
    float ratio = 0.f;
    float speed = 0.f;
    bool automove = false;
    bool moveBackwards = false;
    VEC3 movement = {0.f, 0.f, 0.f};

    VEC3 orbit(float dt, VEC3 targetPos, float rotationSpeed);

public:
    void debugInMenu();
    static void registerMsgs();
    void load(const json& j, TEntityParseContext& ctx);
    void onGroupCreated(const TMsgEntitiesGroupCreated & msg);

    void update(float dt);

    void setRatio(float ratio) { ratio = ratio; }
    VEC3 getMovement() { return movement; }

    TCompTransform* getTransform() { return transformHandle; }
    TCompCollider* getCollider() { return colliderHandle; }
};
