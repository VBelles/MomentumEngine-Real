#pragma once

#include "comp_base.h"
#include "camera/camera.h"

class TCompCamera : public TCompBase {
protected:
	CCamera* camera;

public:
	DECL_SIBLING_ACCESS();
	~TCompCamera();
	void debugInMenu();
	void renderDebug();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);

	CCamera* getCamera();
};