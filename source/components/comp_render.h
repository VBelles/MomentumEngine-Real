#pragma once

#include "comp_base.h"

class CRenderMesh;
class CTexture;
class CRenderTechnique;
class CMaterial;

class TCompRender : public TCompBase {
  void loadMesh(const json& j, TEntityParseContext& ctx);
  CTimer timer;
  float timeToNormal;
  bool isColorChanged = false;
  VEC4 originalColor;
public:
  VEC4               color = VEC4(1, 1, 1, 1);

  // This represents a single object mesh with several materials. Not multiples meshes
  const CRenderMesh* mesh = nullptr;
  std::vector<const CMaterial*> materials;

  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void setMesh(std::string meshName);

  void update(float delta);
  void TurnRed(float time);

  DECL_SIBLING_ACCESS();
};
