#pragma once

#include "components/comp_base.h"
#include "entity/common_msgs.h"
#include "components/comp_transform.h"

class CCurve;

class TCompFollowCurve : public TCompBase {
    DECL_SIBLING_ACCESS();

    const CCurve* _curve = nullptr;
    float _ratio = 0.f;
    float _speed = 0.f;
    bool _automove = false;
    VEC3 _pos;

public:
    void debugInMenu();
    static void registerMsgs();
    void load(const json& j, TEntityParseContext& ctx);
    void onGroupCreated(const TMsgEntitiesGroupCreated & msg);

    void update(float dt);

    void setRatio(float ratio) { _ratio = ratio; }
};
