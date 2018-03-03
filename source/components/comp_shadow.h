#pragma once

#include "comp_base.h"

class CRenderMesh;
class CMaterial;
class TCompTransform;

class TCompShadow : public TCompBase {
	DECL_SIBLING_ACCESS();

	PxReal maxDistance = 10000;
	PxVec3 unitDir = { 0,-1, 0 };

	CHandle transformHandle;
	CHandle parentTransformHandle;

public:
	const CRenderMesh* mesh = nullptr;
	std::vector<const CMaterial*> materials;

	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	static void registerMsgs();

	void onCreate(const TMsgEntityCreated& msg);
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);

	void setMesh(std::string meshName);

	TCompTransform* getTransform();
};