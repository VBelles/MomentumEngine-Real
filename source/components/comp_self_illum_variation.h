#pragma once

#include "comp_base.h"

struct TMsgEntitiesGroupCreated;

class TCompSelfIllumVariation : public TCompBase {
private:
	//json
	bool enabled = true;
	float halfAmplitude = 0.15f;
	float timeFactor = 3.3f;
	float maxRatio = 1.f;

	CTimer timer;

	//Messages
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);

public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);

	void setEnabled(bool enable, float stopRatio = -1.f);
};