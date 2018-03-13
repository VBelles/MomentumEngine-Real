#pragma once

#include "comp_base.h"
#include "camera/camera.h"

class TCompCamera : public CCamera, public TCompBase {
	bool hasTarget = false;
	VEC3 target;
public:
	void debugInMenu();
	void renderDebug();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);

	void SetTarget(VEC3 target);
	DECL_SIBLING_ACCESS();
};