#ifndef _AIC_PATROL
#define _AIC_PATROL

#include "ai_controller.h"
#include "entity/common_msgs.h"

class CAIPatrol : public IAIController
{
  std::vector<VEC3> _waypoints;
  int currentWaypoint;
  float degreesOrbited = 0.f;
  float thisOrbitAngle = 60.f;
  const float maxOrbitAngle = 60.f;
  float orbitAngularVelocity = 0.5f;
  float health = 180.f;

  void OnHit(const TMsgDamage& msg);

  DECL_SIBLING_ACCESS();

public:
  void load(const json& j, TEntityParseContext& ctx);
  void debugInMenu();
  static void registerMsgs();

  void IdleState(float delta);
  void SeekWptState(float delta);
  void NextWptState(float delta);
  void ChaseState(float delta);
  void IdleWar(float delta);
  void OrbitLeft(float delta);
  void OrbitRight(float delta);
  void MoveBack(float delta);

  void Orbit(float increment);

  void Init();

  void addWaypoint(VEC3 waypoint) { _waypoints.push_back(waypoint); };
  VEC3 getWaypoint() { return _waypoints[currentWaypoint]; }
};

#endif