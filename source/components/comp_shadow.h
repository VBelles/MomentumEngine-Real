#pragma once

#include "comp_base.h"

class CRenderMesh;
class CMaterial;
class TCompTransform;
class TCompCollider;

struct TMsgEntityCreated;
struct TMsgEntitiesGroupCreated;

class TCompShadow : public TCompBase {
	DECL_SIBLING_ACCESS();

private:
	std::string shadowMeshName;
	std::string disabledMeshName = "data/meshes/nada.mesh";
	bool enabled = true;

	PxReal maxDistance = 10000;
	PxVec3 unitDir = { 0,-1, 0 };

	CHandle transformHandle;
	CHandle parentTransformHandle;

	VEC3 offset = { 0, 0, 0 };

public:
	const CRenderMesh* mesh = nullptr;
	std::vector<const CMaterial*> materials;

	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	static void registerMsgs();

	void onCreate(const TMsgEntityCreated& msg);
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);

	void setMesh(std::string meshName);

	TCompTransform* getTransform() { return transformHandle; }

	void enable();
	void disable();
};