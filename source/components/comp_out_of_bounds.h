#pragma once

#include "comp_base.h"

class TComputOfBounds : public TCompBase {
	DECL_SIBLING_ACCESS();

public:
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	void onTriggerEnter(const TMsgTriggerEnter& msg);
};