#pragma once

#include "comp_base.h"
#include "geometry/transform.h"

class TCompPlayerModel : public TCompBase {
  float   speedFactor = 2.0f;
  float   rotationSpeed = 2.0f;

  DECL_SIBLING_ACCESS();

public:
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
  void update(float dt);
  void SetTranslationInput(VEC2 input, float delta);
  void SetRotationInput(float amount, float delta);
};