#include "mcv_platform.h"
#include "comp_platform_simple.h"
#include "render/render_objects.h"
#include "components/player/comp_player_model.h"
#include "components/comp_collider.h"
#include "components/comp_transform.h"

DECL_OBJ_MANAGER("platform_simple", TCompPlatformSimple);

void TCompPlatformSimple::debugInMenu() {
	if (ImGui::TreeNode("Waypoints")) {
		for (auto& v : waypoints) {
			ImGui::PushID(&v);
			ImGui::DragFloat3("Point", &v.x, 0.1f, -20.f, 20.f);
			ImGui::PopID();
		}
		ImGui::TreePop();
	}

	for (size_t i = 0; i < waypoints.size(); ++i)
		renderLine(waypoints[i],
			waypoints[(i + 1) % waypoints.size()],
			VEC4(0, 1, 0, 1));
}

void TCompPlatformSimple::registerMsgs() {
	DECL_MSG(TCompPlatformSimple, TMsgEntityCreated, onCreated);
}

void TCompPlatformSimple::load(const json& j, TEntityParseContext& ctx) {
	speed = j.value("speed", 0.f);
	if (j.count("rotation_speed")) {
		rotationSpeed = loadVEC3(j["rotation_speed"]) * (float)M_PI / 180.f;
	}

	auto& jWaypoints = j["waypoints"];
	for (auto& jWaypoint : jWaypoints) {
		VEC3 p = loadVEC3(jWaypoint);
		waypoints.push_back(p);
	}

}

void TCompPlatformSimple::onCreated(const TMsgEntityCreated& msg) {
	transformHandle = get<TCompTransform>();
	assert(transformHandle.isValid());
	colliderHandle = get<TCompCollider>();
	assert(colliderHandle.isValid());
}

void TCompPlatformSimple::update(float delta) {
	TCompTransform* transform = getTransform();
	VEC3 position = transform->getPosition();

	//Position
	float increment = speed * delta;

	if (VEC3::Distance(position, waypoints[currentWaypoint]) <= increment) {
		currentWaypoint = (currentWaypoint + 1) % waypoints.size();
	}

	VEC3 direction = waypoints[currentWaypoint] - position;
	direction.Normalize();

	transform->setPosition(position + direction * increment);

	//Rotation
	VEC3 yawPitchRoll;
	transform->getYawPitchRoll(&yawPitchRoll.x, &yawPitchRoll.y, &yawPitchRoll.z);
	yawPitchRoll += rotationSpeed * delta;
	transform->setYawPitchRoll(yawPitchRoll.x, yawPitchRoll.y, yawPitchRoll.z);

	//Update collider
	getRigidDynamic()->setKinematicTarget(toPhysx(transform));

}

TCompTransform* TCompPlatformSimple::getTransform() { return transformHandle; }
TCompCollider* TCompPlatformSimple::getCollider() { return colliderHandle; }
PxRigidDynamic* TCompPlatformSimple::getRigidDynamic() { return static_cast<PxRigidDynamic*>(getCollider()->actor); }

