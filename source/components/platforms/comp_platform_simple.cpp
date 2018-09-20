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

	ImGui::DragFloat("rollSpeed", &rollSpeed, 0.01f, -20.f, 20.f);
	ImGui::DragFloat("rollWaitDuration", &rollWaitDuration, 0.01f, 0.f, 20.f);

	if (ImGui::Checkbox("Enabled", &enabled)) {
		if (enabled) {
			rollTimer.setElapsed(elapsedAtPauseRoll);
			travelWaitTimer.setElapsed(elapsedAtPauseTravelWait);
		}
		else {
			elapsedAtPauseRoll = rollTimer.elapsed();
			elapsedAtPauseTravelWait = travelWaitTimer.elapsed();
		}
	}
}

void TCompPlatformSimple::setEnabled(bool enabled) {
	if (this->enabled != enabled) {
		this->enabled = enabled;
		if (enabled) {
			rollTimer.setElapsed(elapsedAtPauseRoll);
			travelWaitTimer.setElapsed(elapsedAtPauseTravelWait);
		}
		else {
			elapsedAtPauseRoll = rollTimer.elapsed();
			elapsedAtPauseTravelWait = travelWaitTimer.elapsed();
		}
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
	rotationAxisLocal = j.count("rotation_axis") ? loadVEC3(j["rotation_axis"]) : VEC3::UnitY;
	if (rotationAxisLocal == VEC3::Zero) rotationAxisLocal = VEC3::UnitY;

	// Roll
	if (j.count("roll")) {
		auto& jRoll = j["roll"];
		rollSpeed = jRoll.value("speed", 5.f);
		rollWaitDuration = jRoll.value("wait_duration", rollWaitDuration);
	}

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
		targetRoll = angleInBounds(targetRoll + sign * M_PI, -M_PI, M_PI);
	}

	rollTimer.reset();
}

void TCompPlatformSimple::turnAround() {
	doRoll = true;
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
			if (!hasDirector && rollTimer.elapsed() >= rollWaitDuration) {
				doRoll = true;
			}
			if (doRoll) {
				float targetRollOutOfBounds = targetRoll;
				if (rollSpeed > 0 && roll >= targetRoll) {
					targetRollOutOfBounds += M_PI * 2;
				}
				else if (rollSpeed < 0 && roll <= targetRoll) {
					targetRollOutOfBounds -= M_PI * 2;
				}
				roll = roll + rollSpeed * delta;
				transform->setYawPitchRoll(yaw, pitch, roll);

				if ((rollSpeed > 0 && roll >= targetRollOutOfBounds) || (rollSpeed < 0 && roll <= targetRollOutOfBounds)) {
					//ha llegado a targetRoll
					int sign = rollSpeed < 0 ? sign = -1 : sign = 1;
					roll = targetRoll;
					targetRoll = angleInBounds(targetRoll + sign * M_PI, -M_PI, M_PI);
					rollTimer.reset();
					doRoll = false;
					transform->setYawPitchRoll(yaw, pitch, roll);
				}
			}

		}
		//Update collider
		auto collider = getRigidDynamic();
		if (collider) {
			collider->setKinematicTarget(toPxTransform(transform));
		}
	}
}

float TCompPlatformSimple::angleInBounds(float angle, float lowerBound, float upperBound) {
	float range = upperBound - lowerBound;
	if (angle > upperBound) {
		angle -= range;
	}
	else if (angle < lowerBound) {
		angle += range;
	}
	return angle;
}

bool TCompPlatformSimple::isRolling() {
	return doRoll;
}


TCompTransform* TCompPlatformSimple::getTransform() { return transformHandle; }
TCompCollider* TCompPlatformSimple::getCollider() { return colliderHandle; }
PxRigidDynamic* TCompPlatformSimple::getRigidDynamic() { return static_cast<PxRigidDynamic*>(getCollider()->actor); }
