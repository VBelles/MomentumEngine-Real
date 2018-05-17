#include "mcv_platform.h"
#include "comp_lod.h"
#include "entity/entity_parser.h"

DECL_OBJ_MANAGER("lod", TCompLOD);

void TCompLOD::debugInMenu() {
  for (auto h : handles)
    h.debugInMenu();
}

TCompLOD::~TCompLOD() {
}

void TCompLOD::load(const json& j, TEntityParseContext& ctx) {
  names = j.value< std::vector<std::string> >("children", {} );
  for (auto& n : names) {
    CHandle h = ctx.findEntityByName(n);
    assert(h.isValid());
    handles.push_back(h);
  }
}