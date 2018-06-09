#pragma once

#include "components/comp_base.h"

class TCompTransform;
class CRenderMesh;

class TCompSlash: public TCompBase {
private:
	float width = 0.f;
	float time = 0.f;
	float minVertexDistanceSquared = 0.f;
	float maxVertex = 0.f;
	VEC3 offset;
	std::string targetName;
	CHandle targetTransformHandle;
	CHandle renderHandle;

	bool enabled = false;

	//std::vector<VEC3> positions;
	std::list<VEC3> positions;

	CRenderMesh* mesh;

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
