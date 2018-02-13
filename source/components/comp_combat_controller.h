#pragma once

#include "comp_base.h"
#include "geometry/transform.h"
#include "entity/common_msgs.h"

class TCompCombatController : public TCompBase {

  CHandle     h_target;
  std::string target_name;
  float       speed = 0.001f;
  float       currYaw = 0.f;
  float       rotatedAmount = 0.f;
  float		  fovAngle = 45.f;
  float		  fovDist = 20.f;
  float		  combatDist = 2.f;
  float		  backDist = 1.f;
  int		  hitPoints = 3;
  bool        orbitLeft = true;

  void onHit(const TMsgDamage& msg);

  DECL_SIBLING_ACCESS();

  typedef void (TCompBase::*statehandler)();

  std::string state;
  std::map<std::string, statehandler>statemap;

public:
  static void registerMsgs();
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);

  void ChangeState(const std::string& newstate);	// state we wish to go to
  virtual void Init();	// resets the controller
  void AddState(const std::string&, statehandler);

  void IdleState();
  void Chase();
  void IdleWar();
  void Orbit();
  void Back();
  void Hit();
};