#include "mcv_platform.h"
#include "comp_platform_simple.h"
#include "render/render_objects.h"
#include "components/player/comp_player_model.h"
#include "components/comp_collider.h"
#include "components/comp_transform.h"
#include "geometry/curve.h"

DECL_OBJ_MANAGER("platform_simple", TCompPlatformSimple);

void TCompPlatformSimple::debugInMenu() {
	ImGui::DragFloat("speed", &speed, 0.01f, 0.f, 20.f);
}

void TCompPlatformSimple::registerMsgs() {
	DECL_MSG(TCompPlatformSimple, TMsgEntityCreated, onCreated);
}

void TCompPlatformSimple::load(const json& j, TEntityParseContext& ctx) {
	//Movement
	speed = j.value("speed", 0.f);
	curve.load(j);
	automove = j.value("automove", false);
	ratio = j.value("move_offset", 0.f); // Starts with an offset (from 0 to 1).

	//Rotation
	rotationSpeed = j.value("rotation_speed", 0.f);
	if (j.count("rotation_axis")) {
		rotationAxis = loadVEC3(j["rotation_axis"]);
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
	if (automove) {
		// Actualizar ratio
		if (!moveBackwards) {
			ratio += speed * delta;
			if (ratio >= 1.f) { // Reaches the end of the spline.
				if (curve.isLooping()) moveBackwards = true;
				else automove = false; // If doesn't loop, stop moving.
			}
		}
		else {
			ratio -= speed * delta;
			if (ratio <= 0.f) moveBackwards = false;
		}
		// Evaluar curva con dicho ratio
		position = curve.evaluate(ratio, position);
		//dbg("posToGo: x: %f y: %f z: %f\n", posToGo.x, posToGo.y, posToGo.z);
	}

	transform->setPosition(position);

	//Rotation
    if (rotationSpeed > 0) {
	    QUAT quat = QUAT::CreateFromAxisAngle(rotationAxis, rotationSpeed * delta);
	    transform->setRotation(transform->getRotation() * quat);
    }

	//Update collider
	getRigidDynamic()->setKinematicTarget(toPhysx(transform));

}

TCompTransform* TCompPlatformSimple::getTransform() { return transformHandle; }
TCompCollider* TCompPlatformSimple::getCollider() { return colliderHandle; }
PxRigidDynamic* TCompPlatformSimple::getRigidDynamic() { return static_cast<PxRigidDynamic*>(getCollider()->actor); }
