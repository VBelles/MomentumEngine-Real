#include "mcv_platform.h"
#include "comp_camera_player.h"
#include "components/comp_camera.h"
#include "components/comp_transform.h"
#include "modules/system/physics/GameQueryFilterCallback.h"

DECL_OBJ_MANAGER("camera_player", TCompCameraPlayer);

void TCompCameraPlayer::debugInMenu() {
	ImGui::DragFloat("distanceToTarget", &defaultDistanceToTarget, 0.1f, 1.f, 1000.f);
}

// -------------------------------------------------
void TCompCameraPlayer::renderDebug() {
}

// -------------------------------------------------
void TCompCameraPlayer::load(const json& j, TEntityParseContext& ctx) {
	targetName = j.value("target", "");
	defaultDistanceToTarget = j.value("distance_to_target", 5.f);
	cameraSpeed = loadVEC2(j["camera_speed"]);
	maxPitch = deg2rad(j.value("max_pitch", 80.f));
	minPitch = deg2rad(j.value("min_pitch", -80.f));
	initialYaw = deg2rad(j.value("yaw", 0.f));
	initialPitch = deg2rad(j.value("pitch", -20.f));
	centeringCameraSpeed = loadVEC2(j["centering_camera_speed"]);
}

void TCompCameraPlayer::registerMsgs() {
	DECL_MSG(TCompCameraPlayer, TMsgEntitiesGroupCreated, onGroupCreated);
	DECL_MSG(TCompCameraPlayer, TMsgLockCameraInput, onLockCameraInput);
}

void TCompCameraPlayer::onLockCameraInput(const TMsgLockCameraInput & msg) {
	isMovementLocked = msg.isLocked;
	centeringCamera = false; //Avoid bugs
}

void TCompCameraPlayer::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	targetHandle = getEntityByName(targetName.c_str());
	transformHandle = get<TCompTransform>();
	assert(targetHandle.isValid());
	assert(transformHandle.isValid());

	getTransform()->setYawPitchRoll(initialYaw, initialPitch);
}

void TCompCameraPlayer::update(float delta) {
	
	updateTargetTransform();
	updateInput();
	if (centeringCamera) {
		updateCenteringCamera(delta);
	}
	else {
		updateMovement(delta);
	}

	if (!isMovementLocked && sphereCast()) {
		sweepBack();
	}
}

void TCompCameraPlayer::updateTargetTransform() {
	//Target transform is player transform + 2y
	TCompTransform* transform = getTarget()->get<TCompTransform>();
	targetTransform.setPosition(transform->getPosition() + VEC3::Up * 2.f);
	targetTransform.setRotation(transform->getRotation());
}

void TCompCameraPlayer::updateInput() {
	input = VEC2::Zero;
	//Hacer sólo si la cámara está mixeada
	if (!isMovementLocked) {
		VEC2 padInput = {
			EngineInput[Input::EPadButton::PAD_RANALOG_X].value,
			EngineInput[Input::EPadButton::PAD_RANALOG_Y].value
		};
		if (padInput.Length() > padDeadZone) {
			input.x -= padInput.x * cameraSpeed.x;
			input.y += padInput.y * cameraSpeed.y;
		}
		else if (!CApp::get().showDebug) {
			auto& mouse = EngineInput[Input::PLAYER_1].mouse();
			input.x -= mouse.position_delta.x * cameraSpeed.x;
			input.y -= mouse.position_delta.y * cameraSpeed.y;
		}
	}
}

void TCompCameraPlayer::updateMovement(float delta) {
	TCompTransform* transform = getTransform();

	//Move the camera to the target position
	transform->setPosition(targetTransform.getPosition());

	//Rotate the camera
	float y, p, r;
	transform->getYawPitchRoll(&y, &p, &r);
	y += input.x * delta;
	p += input.y * delta;
	p = clamp(p, minPitch, maxPitch);
	transform->setYawPitchRoll(y, p, r);

	//Move the camera back
	transform->setPosition(transform->getPosition() - transform->getFront() * defaultDistanceToTarget);
}

void TCompCameraPlayer::updateCenteringCamera(float delta) {
	TCompTransform* transform = getTransform();

	//Move the camera to the target position
	transform->setPosition(targetTransform.getPosition());
	float yaw, pitch, r;
	transform->getYawPitchRoll(&yaw, &pitch, &r);

	VEC2 increment = centeringCameraSpeed * delta;

	//Center yaw
	float difference = atan2(sin(desiredYawPitch.x - yaw), cos(desiredYawPitch.x - yaw));
	if (abs(difference) <= increment.x) {
		yaw = desiredYawPitch.x;
	}
	else {
		int dir = difference > 0 ? 1 : -1;
		yaw += increment.x * dir;
	}
	//Center pitch
	if (abs(desiredYawPitch.y - pitch) <= increment.y) {
		pitch = desiredYawPitch.y;

	}
	else {
		int dir = pitch < desiredYawPitch.y ? 1 : -1;
		pitch += increment.y * dir;
	}

	if (yaw == desiredYawPitch.x && pitch == desiredYawPitch.y) {
		centeringCamera = false;
	}

	transform->setYawPitchRoll(yaw, pitch, r);

	//Move the camera back
	transform->setPosition(transform->getPosition() - transform->getFront() * defaultDistanceToTarget);

}
void TCompCameraPlayer::sweepBack() {
	VEC3 targetPosition = targetTransform.getPosition();
	VEC3 position = getTransform()->getPosition();
	QUAT rotation = getTransform()->getRotation();
	VEC3 direction = position - targetPosition;
	direction.Normalize();
	PxSphereGeometry geometry(sphereCastRadius);
	PxReal distance = defaultDistanceToTarget;
	PxSweepBuffer sweepBuffer;
	PxQueryFilterData fd;
	fd.data = PxFilterData(EnginePhysics.Player, EnginePhysics.Scenario, 0, 0);
	fd.flags |= PxQueryFlag::eANY_HIT | PxQueryFlag::ePREFILTER;
	PxHitFlags hitFlags = PxHitFlag::eDEFAULT;

	bool status = EnginePhysics.getScene()->sweep(geometry, toPhysx(targetPosition, rotation), toPhysx(direction), distance, sweepBuffer,
		hitFlags, fd, EnginePhysics.getGameQueryFilterCallback());

	if (status) {
		PxSweepHit& hit = sweepBuffer.block;
		PxVec3 newPosition = hit.position + (hit.normal * sphereCastRadius);
		getTransform()->setPosition(fromPhysx(newPosition));
	}
}

bool TCompCameraPlayer::sphereCast() {
	PxSphereGeometry sphereShape(sphereCastRadius); //shape to test for overlaps
	PxTransform pxTransform(toPhysx(getTransform()));
	PxOverlapBuffer buf;
	PxQueryFilterData fd;
	fd.data = PxFilterData(EnginePhysics.Player, EnginePhysics.Scenario, 0, 0);
	fd.flags |= PxQueryFlag::eANY_HIT | PxQueryFlag::ePREFILTER;
	bool status = EnginePhysics.getScene()->overlap(sphereShape, pxTransform, buf, fd, EnginePhysics.getGameQueryFilterCallback());
	return status;
}


void TCompCameraPlayer::CenterCamera() {
	if (!isMovementLocked) {
		centeringCamera = true;
		VEC3 front = targetTransform.getFront();
		desiredYawPitch = VEC2(atan2(front.x, front.z), initialPitch);
	}
}

CEntity* TCompCameraPlayer::getTarget() {
	return targetHandle;
}


TCompTransform* TCompCameraPlayer::getTransform() {
	return transformHandle;
}