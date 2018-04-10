#pragma once

#include "comp_base.h"
 
class TCompRespawner : public TCompBase {

private:
	CTimer timer;
	float respawnTime = 5.f;
	bool isDead = false;

public:
  void load(const json& j, TEntityParseContext& ctx);
  void update(float delta);
  void onDead();
};