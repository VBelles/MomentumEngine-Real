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

	struct TVtxPosNUv {
		VEC3 pos;
		VEC3 normal;
		VEC2 uv;
		TVtxPosNUv() {}
		TVtxPosNUv(VEC3 pos, VEC3 normal, VEC2 uv) : pos(pos), normal(normal), uv(uv) {}
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
	VEC3 rotOffset;
	float tailMultiplier = 0.f;
	float headMultiplier = 0.f;
	float duration = 0.f;

	CHandle renderHandle;
	CHandle skeletonHandle;

	std::list<TControlPoint> points;

	TCompTransform* getTargetTransform();
	TCompSkeleton* getSkeleton();

public:
	CRenderMesh * mesh = nullptr;
	const CTexture* texture = nullptr;

	DECL_SIBLING_ACCESS();

	~TCompSlash();

	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);

	void updatePoints(float delta);
	void updateMesh();

	void onAllScenesCreated(const TMsgAllScenesCreated & msg);

	void setEnable(bool enabled);
};
