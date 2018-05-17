#include "mcv_platform.h"
#include "comp_lod.h"
#include "entity/entity.h"
#include "entity/entity_parser.h"
#include "components/comp_render.h"
#include "components/comp_aabb.h"
#include "components/comp_transform.h"
#include "components/comp_camera.h"
#include "render/render_manager.h"

DECL_OBJ_MANAGER("lod", TCompLOD);

void TCompLOD::debugInMenu() {
  ImGui::DragFloat("Threshold", &lod_threshold, 0.001f, 0.0f, 0.1f);
  ImGui::Text("Current: %f", lod_level);
  ImGui::Text("Using LOD: %s", using_lod ? "YES" : "NO");
  ImGui::Text("Distance To Camera: %f", distance_to_camera);
  ImGui::Text("My Size: %f", my_size);
  
  ImGui::InputFloat3("AABB.center", &aabb.Center.x);
  ImGui::InputFloat3("AABB.half", &aabb.Extents.x);

  if (ImGui::TreeNode("Children")) {
    for (auto h : handles)
      h.debugInMenu();
    ImGui::TreePop();
  }
}

void TCompLOD::load(const json& j, TEntityParseContext& ctx) {
  names = j.value< std::vector<std::string> >("children", {} );
  for (auto& n : names) {
    CHandle h = ctx.findEntityByName(n);
    assert(h.isValid());
    handles.push_back(h);
  }
  camera_name = j.value( "camera", "test_camera_flyover" );
  lod_threshold = j.value("threshold", lod_threshold);
  h_camera = ctx.findEntityByName(camera_name);
}

void TCompLOD::updateAABBFromChildren() {

  int nchildren = 0;

  for (auto h : handles) {
    CEntity* e = h;
    TCompAbsAABB* c_aabb = e->get<TCompAbsAABB>();
    if (c_aabb) {
      AABB* child_aabb = c_aabb;
      if (nchildren == 0)
        aabb = *child_aabb;
      else
        AABB::CreateMerged(aabb, aabb, *child_aabb);
      ++nchildren;
    }
  }

}

// 
void TCompLOD::updateLevel() {
  
  // Compute a ratio between my size and the distance to the camera
  VEC3 my_half_size = aabb.Extents;
  my_size = my_half_size.Length();

  TCompTransform* my_trans = get<TCompTransform>();
  assert(my_trans);

  //Engine.getRender().getName.getCameras().setOutputCamera
  CEntity* e_camera = h_camera;
  if (!e_camera)
    return;
  assert(e_camera);
  TCompCamera* camera_render = e_camera->get<TCompCamera>();
  assert(camera_render);

  VEC3 camera_pos = camera_render->getPosition();
  VEC3 my_pos = my_trans->getPosition();
  distance_to_camera = (camera_pos - my_pos).Length();

  lod_level = my_size / distance_to_camera;
  using_lod = lod_level < lod_threshold;
}

void TCompLOD::applyLODStatus() {
  // this.visible( using_lod );
  // children.visible( !using_lod );
}

void TCompLOD::update(float delta) {
  updateAABBFromChildren();
  
  h_camera = ::getEntityByName(camera_name);

  bool was_using_lod = using_lod;
  updateLevel();

  if (was_using_lod != using_lod)
    applyLODStatus();

}

