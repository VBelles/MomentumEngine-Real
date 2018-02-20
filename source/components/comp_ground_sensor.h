#pragma once

#include "comp_base.h"
#include "geometry/transform.h"
#include "entity/common_msgs.h"
#include "PxPhysicsAPI.h"

class TCompGroundSensor : public TCompCollider {
  DECL_SIBLING_ACCESS();


public:
  static void registerMsgs();
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);
  void onCreate(const TMsgEntityCreated& msg);
  void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
};