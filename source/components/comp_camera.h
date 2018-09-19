#pragma once

#include "comp_base.h"
#include "camera/camera.h"

class TCompCamera : public TCompBase {
private:
	bool gameCamera = false;

protected:
	CCamera* camera = nullptr;

public:
	DECL_SIBLING_ACCESS();
	~TCompCamera();
	void debugInMenu();
	void renderDebug();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);

	CCamera* getCamera();
};