#pragma once

#include "comp_base.h"

class TCompShadow : public TCompBase {
	DECL_SIBLING_ACCESS();

public:
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
};