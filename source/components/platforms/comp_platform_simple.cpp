#include "mcv_platform.h"
#include "comp_platform_simple.h"
#include "render/render_objects.h"
#include "components/player/comp_player_model.h"
#include "components/comp_collider.h"
#include "components/comp_transform.h"
#include "geometry/curve.h"

DECL_OBJ_MANAGER("platform_simple", TCompPlatformSimple);

void TCompPlatformSimple::debugInMenu() {
	if (ImGui::DragFloat("ratio", &ratio, 0.01f, 0.f, 1.f)) {
		TCompTransform* transform = getTransform();
		transform->setPosition(curve.evaluate(ratio, transform->getPosition()));
	}

	ImGui::DragFloat("speed", &speed, 0.01f, 0.f, 20.f);
	ImGui::Checkbox("automove", &automove);
	ImGui::Checkbox("moveBackwards", &moveBackwards);
	bool isLooping = curve.isLooping();
	ImGui::Checkbox("loop", &isLooping);
	curve.setLoop(isLooping);


	ImGui::DragFloat("rotationSpeed", &rotationSpeed, 0.001f, 0.f, 2.f);
	if (ImGui::DragFloat3("rotationAxis", &rotationAxisLocal.x, 0.5f, -1.f, 1.f)) {
		TCompTransform* transform = get<TCompTransform>();
		rotationAxisGlobal = transform->getLeft() * rotationAxisLocal.x + transform->getUp() * rotationAxisLocal.y + transform->getFront() * rotationAxisLocal.z;
	}
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
	if (curve.getType() == CCurve::EType::CIRCULAR) {
		isClosed = true;
	}
	else {
		isClosed = j.value("closed", false);
	}

	//Rotation
	rotationSpeed = j.value("rotation_speed", 0.f);
	if (j.count("rotation_axis")) {
		rotationAxisLocal = loadVEC3(j["rotation_axis"]);
	}
}

void TCompPlatformSimple::onCreated(const TMsgEntityCreated& msg) {
	TCompTransform* transform = get<TCompTransform>();
	//combinar up/left/front para encontrar la rotationAxisGlobal
	rotationAxisGlobal = transform->getLeft() * rotationAxisLocal.x + transform->getUp() * rotationAxisLocal.y + transform->getFront() * rotationAxisLocal.z;
	transformHandle = CHandle(transform);
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
		int sign = moveBackwards ? -1 : 1;
		ratio += speed * delta * sign;
		if (ratio >= 1.f || ratio <= 0.f) { // Reaches the end of the spline. 
			if (curve.isLooping()){
				if (isClosed) {
					ratio = ratio - floor(ratio);
				}
				else {
					moveBackwards = !moveBackwards;
					ratio = clamp(ratio, 0.f, 1.f);
				}
			}
			else {
				automove = false;
				moveBackwards = !moveBackwards;
				ratio = clamp(ratio, 0.f, 1.f);
			}
		}



		//if (!moveBackwards) {
		//	ratio += speed * delta;
		//	if (ratio >= 1.f) { // Reaches the end of the spline.
		//		if (curve.isLooping()) {
		//			if (isClosed) {
		//				ratio = ratio - floor(ratio);
		//			}
		//			else {
		//				moveBackwards = true;
		//				ratio = 1.f;
		//			}
		//		}
		//		else {
		//			automove = false; // If doesn't loop, stop moving.
		//			ratio = 1.f;
		//		}
		//	}
		//}
		//else {
		//	ratio -= speed * delta;
		//	if (ratio <= 0.f) {
		//		moveBackwards = false;
		//		ratio = 0.f;
		//	}

		//}
		// Evaluar curva con dicho ratio
		position = curve.evaluate(ratio, position);
		//dbg("posToGo: x: %f y: %f z: %f\n", posToGo.x, posToGo.y, posToGo.z);
		transform->setPosition(position);
	}


	//Rotation
	if (rotationSpeed > 0) {
		QUAT quat = QUAT::CreateFromAxisAngle(rotationAxisGlobal, rotationSpeed * delta);
		transform->setRotation(transform->getRotation() * quat);
	}

	//Update collider
	getRigidDynamic()->setKinematicTarget(toPxTransform(transform));

}

TCompTransform* TCompPlatformSimple::getTransform() { return transformHandle; }
TCompCollider* TCompPlatformSimple::getCollider() { return colliderHandle; }
PxRigidDynamic* TCompPlatformSimple::getRigidDynamic() { return static_cast<PxRigidDynamic*>(getCollider()->actor); }
