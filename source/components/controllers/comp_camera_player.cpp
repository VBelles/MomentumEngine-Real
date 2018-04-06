#include "mcv_platform.h"
#include "comp_camera_player.h"
#include "../comp_camera.h"
#include "components/comp_transform.h"

DECL_OBJ_MANAGER("camera_player", TCompCameraPlayer);

void TCompCameraPlayer::debugInMenu() {
	ImGui::DragFloat("distanceToTarget", &defaultDistanceToTarget, 0.1f, 1.f, 1000.f);
}

// -------------------------------------------------
void TCompCameraPlayer::renderDebug() {
}

// -------------------------------------------------
void TCompCameraPlayer::load(const json& j, TEntityParseContext& ctx) {
	defaultDistanceToTarget = j.value("distance_to_target", 5.f);
	distanceVector.z = -defaultDistanceToTarget;
	currentDistanceToTarget = defaultDistanceToTarget;
	targetName = j.value("target", "");
}

void TCompCameraPlayer::registerMsgs() {
	DECL_MSG(TCompCameraPlayer, TMsgEntitiesGroupCreated, OnGroupCreated);
	DECL_MSG(TCompCameraPlayer, TMsgLockCameraInput, OnLockCameraInput);
}

void TCompCameraPlayer::OnLockCameraInput(const TMsgLockCameraInput & msg) {
	isMovementLocked = msg.isLocked;
}

void TCompCameraPlayer::OnGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	targetHandle = getEntityByName(targetName.c_str());
	transformHandle = get<TCompTransform>();

	GetTransform()->setYawPitchRoll(0, DEFAULT_Y, 0);
	pitchAngleRange = Y_ANGLE_MAX - Y_ANGLE_MIN;
}

void TCompCameraPlayer::update(float delta) {
	VEC2 increment = GetIncrementFromInput(delta);
	UpdateMovement(increment, delta);
	CalculateVerticalOffsetVector();



	PxOverlapBuffer hitBuffer;            // [out] Overlap results
	if (SphereCast(hitBuffer)) {
		//PxOverlapHit firstHit = hitBuffer.getTouch(0);
		//En teoria hauriem de passar a la següent funció la posició del hit
		AproachToFreePosition();
	}
	else {
		//Tirar cámara siempre hacia defaultDistance
		if (currentDistanceToTarget != defaultDistanceToTarget) {
			float difference = defaultDistanceToTarget - currentDistanceToTarget;
			float zoomIncrement = zoomSpeed * delta;
			if (difference <= zoomIncrement) {
				currentDistanceToTarget = defaultDistanceToTarget;
			}
			else {
				int dir = difference < 0 ? -1 : 1;
				currentDistanceToTarget += zoomIncrement * dir;
			}
		}
	}
}

bool TCompCameraPlayer::SphereCast(PxOverlapBuffer buf) {
	VEC3 position = GetTransform()->getPosition();
	QUAT rotation = GetTransform()->getRotation();
	VEC3 front = GetTransform()->getFront();
	PxSphereGeometry sphereShape(sphereCastRadius); //shape to test for overlaps
	PxTransform pxTransform; // initial shape pose (at distance=0)
	pxTransform.p = PxVec3(position.x + front.x * sphereCastRadius, position.y + front.x * sphereCastRadius, position.z + front.z * sphereCastRadius);
	pxTransform.q = PxQuat(rotation.x, rotation.y, rotation.z, rotation.w);
	PxQueryFilterData fd;
	fd.flags |= PxQueryFlag::eANY_HIT;
	bool status = EnginePhysics.getScene()->overlap(sphereShape, pxTransform, buf, fd);
	bool touched = false;
	for (PxU32 i = 0; i < buf.getNbAnyHits(); i++) {
		auto& hit = buf.getAnyHit(i);
		if (!hit.shape->getFlags().isSet(PxShapeFlag::eTRIGGER_SHAPE)) {
			touched = true;
		}
	}
	return touched;
}

void TCompCameraPlayer::AproachToFreePosition() {
	VEC3 targetPosition = GetTargetPosition();
	VEC3 front = GetTransform()->getFront();
	PxVec3 raycastOrigin = PxVec3(targetPosition.x, targetPosition.y, targetPosition.z);
	PxVec3 raycastDirection = PxVec3(-front.x, -front.y, -front.z);
	PxReal raycastMaxDistance = VEC3::Distance(GetTransform()->getPosition(), targetPosition);

	const PxU32 bufferSize = 256;
	PxRaycastHit hitBuffer[bufferSize];
	PxRaycastBuffer buf(hitBuffer, bufferSize);

	bool status = EnginePhysics.getScene()->raycast(raycastOrigin, raycastDirection, raycastMaxDistance, buf);
	if (!status) return;

	float nearestDistance = raycastMaxDistance;
	PxRaycastHit nearestHit;
	for (PxU32 i = 0; i < buf.nbTouches; i++) {
		PxRaycastHit& touch = buf.touches[i];
		if (!touch.shape->getFlags().isSet(PxShapeFlag::eTRIGGER_SHAPE)) {
			if (touch.distance < nearestDistance) {
				nearestDistance = touch.distance;
				nearestHit = touch;
			}
		}
	}

	VEC3 newPosition = VEC3(nearestHit.position.x, nearestHit.position.y, nearestHit.position.z) + front * (sphereCastRadius);
	GetTransform()->setPosition(newPosition);
	currentDistanceToTarget = VEC3::Distance(GetTransform()->getPosition(), targetPosition);
}

VEC2 TCompCameraPlayer::GetIncrementFromInput(float delta) {
	VEC2 increment = VEC2::Zero;
	//Hacer sólo si la cámara está mixeada
	if (!isMovementLocked) {
		VEC2 padInput = {
			EngineInput[Input::EPadButton::PAD_RANALOG_X].value,
			EngineInput[Input::EPadButton::PAD_RANALOG_Y].value
		};
		if (padInput.Length() > padDeadZone) {
			increment.x -= padInput.x * cameraSpeed.x * delta;
			increment.y += padInput.y * cameraSpeed.y * delta;
		}
		else if (!CApp::get().showDebug) {
			auto& mouse = EngineInput[Input::PLAYER_1].mouse();
			increment.x -= mouse.position_delta.x * cameraSpeed.x * delta;
			increment.y -= mouse.position_delta.y * cameraSpeed.y * delta;
		}
	}
	return increment;
}

void TCompCameraPlayer::UpdateMovement(VEC2 increment, float delta) {
	float y, p, r;
	GetTransform()->getYawPitchRoll(&y, &p, &r);

	//Move the camera to the target position
	GetTransform()->setPosition(GetTargetPosition());

	//Rotate the camera
	y += increment.x;
	p += increment.y;
	p = clamp(p, Y_ANGLE_MIN, Y_ANGLE_MAX);
	GetTransform()->setYawPitchRoll(y, p, r);

	//Move the camera back
	GetTransform()->setPosition(GetTransform()->getPosition() - GetTransform()->getFront() * currentDistanceToTarget);
}

void TCompCameraPlayer::CalculateVerticalOffsetVector() {
	float y, p, r;
	TCompTransform* transform = get<TCompTransform>();
	transform->getYawPitchRoll(&y, &p, &r);
	float currentOffset = ((pitchAngleRange - (y - Y_ANGLE_MIN)) / pitchAngleRange) * (maxVerticalOffset - minVerticalOffset) + minVerticalOffset;
	verticalOffsetVector.y = currentOffset;
}

void TCompCameraPlayer::CenterCamera() {
	if (!isMovementLocked) {
		centeredPosition = GetTargetPosition() - GetTargetTransform()->getFront() * currentDistanceToTarget;
		VEC3 velocityVector = GetTargetPosition() - centeredPosition;
		velocityVector.Normalize();

		GetTransform()->setPosition(centeredPosition);
		float y, p, r;
		GetTransform()->getYawPitchRoll(&y, &p, &r);
		y = atan2(velocityVector.x, velocityVector.z);
		//p = asin(-velocityVector.y); //No nos interesa el pitch
		GetTransform()->setYawPitchRoll(y, p, r);
	}
}

CEntity* TCompCameraPlayer::GetTarget() {
	return targetHandle;
}

TCompTransform* TCompCameraPlayer::GetTargetTransform() {
	return GetTarget()->get<TCompTransform>();
}

VEC3 TCompCameraPlayer::GetTargetPosition() {
	return GetTargetTransform()->getPosition() + VEC3::Up * 2.f;
}

TCompTransform* TCompCameraPlayer::GetTransform() {
	return transformHandle;
}