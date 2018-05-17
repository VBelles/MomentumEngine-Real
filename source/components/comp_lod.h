#ifndef INC_COMP_LOD_H_
#define INC_COMP_LOD_H_

#include "comp_base.h"

class CTransform;

struct TCompLOD : public TCompBase {
  VHandles handles;
  std::vector< std::string > names;
  ~TCompLOD();
  void debugInMenu();
  void load(const json& j, TEntityParseContext& ctx);
};

#endif
