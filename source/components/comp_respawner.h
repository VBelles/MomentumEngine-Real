#pragma once

#include "comp_base.h"
 
class TCompRespawner : public TCompBase {
	CTimer timer;
	float respawnTime = 5.f;
	bool isDead = false;
public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float delta);
  void OnDead();
};