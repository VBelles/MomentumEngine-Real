#include "mcv_platform.h"
#include "comp_camera_player.h"
#include "../comp_camera.h"
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
	defaultDistanceToTarget = j.value("distance_to_target", 5.f);
	distanceVector.z = -defaultDistanceToTarget;
	currentDistanceToTarget = defaultDistanceToTarget;
	targetName = j.value("target", "");
}

void TCompCameraPlayer::registerMsgs() {
	DECL_MSG(TCompCameraPlayer, TMsgEntitiesGroupCreated, OnGroupCreated);
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
	VEC3 increment = GetTransform()->getFront() * sphereCastRadius;
	VEC3 nextPosition = GetTransform()->getPosition() + increment;

	PxSphereGeometry sphereShape(sphereCastRadius); //shape to test for overlaps
	PxTransform pxTransform(toPhysx(nextPosition, GetTransform()->getRotation()));

	PxQueryFilterData fd;
	fd.data = PxFilterData(EnginePhysics.Player, EnginePhysics.Scenario, 0,0);
	fd.flags |= PxQueryFlag::eANY_HIT | PxQueryFlag::ePREFILTER;
	bool status = EnginePhysics.getScene()->overlap(sphereShape, pxTransform, buf, fd, EnginePhysics.getGameQueryFilterCallback());
	return status;
}

void TCompCameraPlayer::AproachToFreePosition() {
	VEC3 targetPosition = GetTargetPosition();
	VEC3 front = GetTransform()->getFront();
	PxVec3 raycastOrigin = toPhysx(targetPosition);
	PxVec3 raycastDirection = toPhysx(-front);
	PxReal raycastMaxDistance = VEC3::Distance(GetTransform()->getPosition(), targetPosition);


	PxRaycastBuffer rayCastBuffer;

	PxQueryFilterData fd;
	fd.data = PxFilterData(EnginePhysics.Player, EnginePhysics.Scenario, 0, 0);
	fd.flags |= PxQueryFlag::eANY_HIT | PxQueryFlag::ePREFILTER;

	bool status = EnginePhysics.getScene()->raycast(raycastOrigin, raycastDirection, raycastMaxDistance, rayCastBuffer,
		PxHitFlag::eDEFAULT, fd, EnginePhysics.getGameQueryFilterCallback());
	
	if (!status) return;

	PxRaycastHit nearestHit = rayCastBuffer.block;
	
	VEC3 newPosition = fromPhysx(nearestHit.position) + front * (sphereCastRadius);
	GetTransform()->setPosition(newPosition);
	currentDistanceToTarget = VEC3::Distance(GetTransform()->getPosition(), targetPosition);
}

VEC2 TCompCameraPlayer::GetIncrementFromInput(float delta) {
	VEC2 increment = VEC2::Zero;
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