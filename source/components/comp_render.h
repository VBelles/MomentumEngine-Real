#pragma once

#include "comp_base.h"

class CRenderMesh;
class CTexture;
class CRenderTechnique;
class CMaterial;
struct TEntityParseContext;
struct TMsgDefineLocalAABB;

class TCompRender : public TCompBase {
	DECL_SIBLING_ACCESS();

private:
	void loadMesh(const json& j, TEntityParseContext& ctx);
	void onDefineLocalAABB(const TMsgDefineLocalAABB& msg);

	CTimer timer;
	float timeToNormal;
	bool isColorChanged = false;
	VEC4 originalColor;

public:
	~TCompRender();

	VEC4               color = VEC4(1, 1, 1, 1);
	AABB               aabb;
	// This represents a single object mesh with several materials. Not multiples meshes
	struct CMeshWithMaterials {
		bool               enabled = true;
		const CRenderMesh* mesh = nullptr;
		std::vector<const CMaterial*> materials;
	};
	const CRenderMesh* mesh = nullptr; // !!! Esto ahora debería ser reemplazado por la struct de arriba.
	const CMaterial* material = nullptr; // !!! Esto ahora debería ser reemplazado por la struct de arriba.
	std::vector<const CMaterial*> materials; // !!! Esto ahora debería ser reemplazado por la struct de arriba.

	std::vector<CMeshWithMaterials> meshes;

	void refreshMeshesInRenderManager();
	void debugInMenu();
	void renderDebug();
	void load(const json& j, TEntityParseContext& ctx);

	void setMesh(std::string meshName, std::string materialName = "");

	void update(float delta);
	void TurnRed(float time);

	static void registerMsgs();

	void enable();
	void disable();

};
