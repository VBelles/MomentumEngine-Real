#include "mcv_platform.h"
#include "comp_rigid_anim.h"
#include "components/comp_transform.h"

DECL_OBJ_MANAGER("player_input", TCompRigidAnim);

void TCompRigidAnim::load(const json& j, TEntityParseContext& ctx) {
  controller.track_name = j.value( "track", "" );
//  controller.track_index = j.value
  controller.anims = Resources.get(j["anims"])->as<RigidAnims::CRigidAnimResource>();
  current_time = 0;
  speed_factor = j.value("speed_factor", 1.0f);
}

void TCompRigidAnim::debugInMenu() {
  ImGui::DragFloat("Time", &current_time, 0.01f, 0.f, 10.0f);
  ImGui::DragFloat("Speed Factor", &speed_factor, 0.01f, 0.f, 5.0f);
}

void TCompRigidAnim::update(float dt) {

  if (!is_moving)
    return;

  // Sample the animation in the current time
  RigidAnims::TKey k;
  bool has_finished = controller.sample(&k, current_time);

  // Transfer the key data to the comp transform
  TCompTransform* c_trans = get< TCompTransform >();
  c_trans->setPosition(k.pos);
  c_trans->setRotation(k.rot);
  c_trans->setScale(k.scale);

  if (has_finished) {
    // loop, change direction?, set is_moving = false...
  }

  // Advance the time
  current_time += dt * speed_factor;
}
