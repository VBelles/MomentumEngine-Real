#pragma once

#include "components/comp_base.h"
#include "entity/common_msgs.h"
#include "components/comp_transform.h"

class CCurve;

class TCompFollowCurve : public TCompBase {
    DECL_SIBLING_ACCESS();

    std::unique_ptr<CCurve> _curve = std::make_unique<CCurve>();
    float _ratio = 0.f;
    float _speed = 0.f;
    bool _automove = false;
    bool _moveBackwards = false;
    VEC3 _movement = {0.f, 0.f, 0.f};

public:
    void debugInMenu();
    static void registerMsgs();
    void load(const json& j, TEntityParseContext& ctx);
    void onGroupCreated(const TMsgEntitiesGroupCreated & msg);

    void update(float dt);

    void setRatio(float ratio) { _ratio = ratio; }
    VEC3 getMovement() { return _movement; }
};
