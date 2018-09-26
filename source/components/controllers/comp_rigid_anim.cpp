#include "mcv_platform.h"
#include "comp_rigid_anim.h"
#include "entity/entity_parser.h"
#include "geometry/rigid_anim.h"

DECL_OBJ_MANAGER("rigid_anim", TCompRigidAnim);

void TCompRigidAnim::load(const json& j, TEntityParseContext& ctx) {
	controller.track_name = j.value("track", "");
	if (controller.track_name.empty()) {
		CEntity* e = ctx.current_entity;
		assert(e);
		controller.track_name = e->getName();
	}
	assert(j.count("src") > 0);
	controller.anims = Resources.get(j["src"])->as<RigidAnims::CRigidAnimResource>();
	controller.track_index = controller.anims->findTrackIndexByName(controller.track_name);
	assert(controller.track_index != RigidAnims::CController::invalid_track_index);
	current_time = 0;
	speed_factor = j.value("speed_factor", speed_factor);
	loops = j.value("loops", loops);
	is_moving = j.value("is_moving", is_moving);
	killOnFinishAnimation = j.value("kill_on_finish", killOnFinishAnimation);
}

void TCompRigidAnim::debugInMenu() {
	ImGui::DragFloat("Time", &current_time, 0.01f, 0.f, 10.0f);
	ImGui::DragFloat("Speed Factor", &speed_factor, 0.01f, 0.f, 5.0f);
	ImGui::Checkbox("Loops", &loops);
	ImGui::Checkbox("Moving", &is_moving);
	ImGui::Text("Track %d %s", controller.track_index, controller.track_name.c_str());
	if (controller.anims)
		((RigidAnims::CRigidAnimResource*) controller.anims)->debugInMenu();
}

void TCompRigidAnim::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	TCompTransform* transform = get<TCompTransform>();
	initialTransform = CTransform(*transform);
	updateAnimation();
}

void TCompRigidAnim::registerMsgs() {
	DECL_MSG(TCompRigidAnim, TMsgEntitiesGroupCreated, onGroupCreated);
}

void TCompRigidAnim::setIsMoving(bool isMoving) {
	is_moving = isMoving;
	stopOnNextLoop = !isMoving;
}

void TCompRigidAnim::setStopOnNextLoop(bool stop) {
	stopOnNextLoop = stop;
}

void TCompRigidAnim::update(float dt) {
	if (!is_moving) return;

	updateAnimation();

	// Advance the time
	current_time += dt * speed_factor;
}

void TCompRigidAnim::updateAnimation() {
	// Sample the animation in the current time
	RigidAnims::TKey k;
	bool has_finished = controller.sample(&k, current_time);

	// Transfer the key data to the comp transform
	MAT44 res = MAT44::CreateFromQuaternion(k.rot)
		* MAT44::CreateTranslation(k.pos)
		* MAT44::CreateFromQuaternion(initialTransform.getRotation())
		* MAT44::CreateTranslation(initialTransform.getPosition());

	TCompTransform* transform = get< TCompTransform >();
	if (!transform) return;
	transform->setScale(k.scale * initialTransform.getScale());
	transform->setPosition(res.Translation());
	transform->setRotation(QUAT::CreateFromRotationMatrix(res));

	if (has_finished) {
		if (loops) {
			current_time = 0;
			if (stopOnNextLoop) {
				is_moving = false;
				if (killOnFinishAnimation) {
					CHandle(this).getOwner().destroy();
				}
			}
		}
		else {
			is_moving = false;
			if (killOnFinishAnimation) {
				CHandle(this).getOwner().destroy();
			}
		}
		// loop, change direction?, set is_moving = false...
	}
}
