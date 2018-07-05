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
	void onSetVisible(const TMsgSetVisible& msg);

	CTimer timer;
	float timeToNormal;
	bool isColorChanged = false;
	VEC4 originalColor;

public:
	~TCompRender();

	VEC4               color = VEC4(1, 1, 1, 1);
	AABB               aabb;
	bool               global_enabled = true;
	// This represents a single object mesh with several materials. Not multiples meshes
	struct CMeshWithMaterials {
		bool               enabled = true;
		const CRenderMesh* mesh = nullptr;
		std::vector<const CMaterial*> materials;
	};
	std::vector<CMeshWithMaterials> meshes;

	void refreshMeshesInRenderManager(bool delete_me_from_keys = true);
	void debugInMenu();
	void renderDebug();
	void load(const json& j, TEntityParseContext& ctx);

	//void setMesh(std::string meshName, std::string materialName = "");

	void setAllMaterials(int startingMesh, int endingMesh, std::string materialName);
	std::vector<std::string> getMaterials(int mesh);
	void setMeshEnabled(int mesh, bool enabled);
	void setAllMaterials(std::vector<std::string> materialNames);

	void update(float delta);
	void TurnRed(float time);

	static void registerMsgs();

	void enable();
	void disable();



};
