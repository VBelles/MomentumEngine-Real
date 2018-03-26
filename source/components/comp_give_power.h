#pragma once

#include "comp_base.h"
 
class TCompGivePower : public TCompBase {
	DECL_SIBLING_ACCESS();
private:	
	float maxPowerToGive = 5000.f;
	float powerToGive = maxPowerToGive;
	float baseMultiplier = 1.0f;
	float stateMultiplier = 1.0f;

	void onGetPower(const TMsgGetPower& msg);

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float delta);
  static void registerMsgs();

  void setStateMultiplier(float multiplier);
  void reset();
};