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
	ImGui::DragFloat("travelWaitTime", &travelWaitTime, 0.01f, 0.f, 20.f);


	ImGui::DragFloat("rotationSpeed", &rotationSpeed, 0.001f, 0.f, 2.f);
	if (ImGui::DragFloat3("rotationAxis", &rotationAxisLocal.x, 0.5f, -1.f, 1.f)) {
		TCompTransform* transform = get<TCompTransform>();
		rotationAxisGlobal = transform->getLeft()  * rotationAxisLocal.x +
							 transform->getUp()    * rotationAxisLocal.y +
							 transform->getFront() * rotationAxisLocal.z;
	}
}

void TCompPlatformSimple::registerMsgs() {
	DECL_MSG(TCompPlatformSimple, TMsgEntityCreated, onCreated);
}

void TCompPlatformSimple::load(const json& j, TEntityParseContext& ctx) {
	// Movement
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
	travelWaitTime = j.value("travel_wait_time", 0.f);

	// Rotation
	rotationSpeed = j.value("rotation_speed", 0.f);
	if (j.count("rotation_axis")) {
		rotationAxisLocal = loadVEC3(j["rotation_axis"]);
	}
	// Roll
	if (j.count("roll")) {
		auto& jRoll = j["roll"];
		rollSpeed = jRoll.value("speed", rollSpeed);
		rollWaitDuration = jRoll.value("wait_duration", rollWaitDuration);
	}
	hasDirector = j.value("has_director", hasDirector);

	enabled = j.value("enabled", enabled);
}

void TCompPlatformSimple::onCreated(const TMsgEntityCreated& msg) {
	TCompTransform* transform = get<TCompTransform>();
	//combinar up/left/front para encontrar la rotationAxisGlobal
	rotationAxisGlobal = transform->getLeft()  * rotationAxisLocal.x +
		transform->getUp()    * rotationAxisLocal.y +
		transform->getFront() * rotationAxisLocal.z;
	transformHandle = CHandle(transform);
	assert(transformHandle.isValid());
	colliderHandle = get<TCompCollider>();
	assert(colliderHandle.isValid());
	if (rollSpeed != 0) {
		float yaw, pitch;
		transform->getYawPitchRoll(&yaw, &pitch, &targetRoll);
		int sign = rollSpeed < 0 ? sign = -1 : sign = 1;
		targetRoll = targetRoll + sign * M_PI;
	}

	rollTimer.reset();
}

void TCompPlatformSimple::turnAround() {
	doRoll = true;
}

void TCompPlatformSimple::setEnabled(bool enabled) {
	this->enabled = enabled;
}

void TCompPlatformSimple::update(float delta) {
	if (enabled) {


		TCompTransform* transform = getTransform();
		VEC3 position = transform->getPosition();

		if (!automove && curve.isLooping()) {
			if (travelWaitTimer.elapsed() >= travelWaitTime) {
				automove = true;
			}
		}

		//Position
		if (automove) {
			// Actualizar ratio
			int sign = moveBackwards ? -1 : 1;
			ratio += speed * delta * sign;
			if (ratio >= 1.f || ratio <= 0.f) { // Reaches the end of the spline. 
				if (curve.isLooping()) {
					if (isClosed) {
						ratio = ratio - floor(ratio);
					}
					else {
						moveBackwards = !moveBackwards;
						ratio = clamp(ratio, 0.f, 1.f);
						automove = false;
						travelWaitTimer.reset();
					}
				}
				else {
					automove = false;
					moveBackwards = !moveBackwards;
					ratio = clamp(ratio, 0.f, 1.f);
				}
			}

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

		if (rollSpeed != 0) {
			float yaw, pitch, roll;
			transform->getYawPitchRoll(&yaw, &pitch, &roll);
			if (hasDirector) {
				if (doRoll) {
					roll = roll + rollSpeed * delta;
					if ((rollSpeed > 0 && roll >= targetRoll) || (rollSpeed < 0 && roll <= targetRoll)) {
						//ha llegado a targetRoll
						int sign = rollSpeed < 0 ? sign = -1 : sign = 1;
						roll = targetRoll;
						targetRoll = targetRoll + sign * M_PI;
						doRoll = false;
					}
					transform->setYawPitchRoll(yaw, pitch, roll);
				}
			}
			else if (rollTimer.elapsed() >= rollWaitDuration) {
				roll = roll + rollSpeed * delta;
				if ((rollSpeed > 0 && roll >= targetRoll) || (rollSpeed < 0 && roll <= targetRoll)) {
					//ha llegado a targetRoll
					int sign = rollSpeed < 0 ? sign = -1 : sign = 1;
					roll = targetRoll;
					targetRoll = targetRoll + sign * M_PI;
					rollTimer.reset();
				}
				transform->setYawPitchRoll(yaw, pitch, roll);
			}
		}
		//Update collider
		getRigidDynamic()->setKinematicTarget(toPxTransform(transform));
	}
}

TCompTransform* TCompPlatformSimple::getTransform() { return transformHandle; }
TCompCollider* TCompPlatformSimple::getCollider() { return colliderHandle; }
PxRigidDynamic* TCompPlatformSimple::getRigidDynamic() { return static_cast<PxRigidDynamic*>(getCollider()->actor); }
