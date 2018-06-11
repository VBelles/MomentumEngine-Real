#pragma once

#include "components/comp_base.h"

class TCompTransform;
class TCompRender;
class CRenderMesh;

class TCompSlash: public TCompBase {
private:
	std::string targetName;
	bool enabled = false;
	float width = 0.f;
	float time = 0.f;
	float minVertexDistanceSquared = 0.f;
	int maxVertex = 0;
	VEC3 offset;
	float tailMultiplier = 0.f;
	float headMultiplier = 0.f;

	CHandle targetTransformHandle;
	CHandle renderHandle;

	std::list<CTransform> points;

	CRenderMesh* mesh = nullptr;

	TCompTransform* getTargetTransform();
	TCompRender* getRender();

public:
	DECL_SIBLING_ACCESS();

    static void registerMsgs();
    void debugInMenu();
    void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);

	void onAllScenesCreated(const TMsgAllScenesCreated & msg);

	void setEnable(bool enabled);
};
