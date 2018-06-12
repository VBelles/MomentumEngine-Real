#pragma once

#include "components/comp_base.h"

class TCompTransform;
class TCompRender;
class TCompSkeleton;
class CRenderMesh;

class TCompSlash : public TCompBase {
private:
	struct TControlPoint {
		TControlPoint(CTransform transform, float time = 0.f) : transform(transform), time(time) {}
		CTransform transform;
		float time = 0.f;
	};

	std::string targetName;
	CHandle targetTransformHandle;
	std::string boneName;
	int boneId = -1;
	bool enabled = false;
	float width = 0.1f;
	float minVertexDistanceSquared = pow(0.25f, 2);
	int maxVertex = 10;
	VEC3 offset;
	float tailMultiplier = 0.f;
	float headMultiplier = 0.f;
	float duration = 1.f;

	CHandle renderHandle;
	CHandle skeletonHandle;

	std::list<TControlPoint> points;

	CRenderMesh* mesh = nullptr;

	TCompTransform* getTargetTransform();
	TCompRender* getRender();
	TCompSkeleton* getSkeleton();

public:
	DECL_SIBLING_ACCESS();

	~TCompSlash();

	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);

	void onAllScenesCreated(const TMsgAllScenesCreated & msg);

	void setEnable(bool enabled);
};
