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
	UpdateTargetTransform();
	VEC2 increment = GetIncrementFromInput(delta);
	UpdateMovement(increment, delta);
	if (!isMovementLocked && SphereCast()) {
		SweepBack();
	}
}

void TCompCameraPlayer::SweepBack() {
	VEC3 targetPosition = targetTransform.getPosition();
	VEC3 position = GetTransform()->getPosition();
	QUAT rotation = GetTransform()->getRotation();
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
		GetTransform()->setPosition(fromPhysx(newPosition));
	}
}

bool TCompCameraPlayer::SphereCast() {

	PxSphereGeometry sphereShape(sphereCastRadius); //shape to test for overlaps
	PxTransform pxTransform(toPhysx(GetTransform()));
	PxOverlapBuffer buf;
	PxQueryFilterData fd;
	fd.data = PxFilterData(EnginePhysics.Player, EnginePhysics.Scenario, 0, 0);
	fd.flags |= PxQueryFlag::eANY_HIT | PxQueryFlag::ePREFILTER;
	bool status = EnginePhysics.getScene()->overlap(sphereShape, pxTransform, buf, fd, EnginePhysics.getGameQueryFilterCallback());
	return status;
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
	TCompTransform* transform = GetTransform();

	float y, p, r;
	transform->getYawPitchRoll(&y, &p, &r);
	transform->setPosition(targetTransform.getPosition());
	//Move the camera to the target position
	//Rotate the camera
	y += increment.x;
	p += increment.y;
	p = clamp(p, Y_ANGLE_MIN, Y_ANGLE_MAX);
	//dbg("p: %f\n", rad2deg(p));
	transform->setYawPitchRoll(y, p, r);
	//Move the camera back
	transform->setPosition(transform->getPosition() - transform->getFront() * currentDistanceToTarget);

}

void TCompCameraPlayer::CalculateVerticalOffsetVector() {
	float y, p, r;
	TCompTransform* transform = get<TCompTransform>();
	transform->getYawPitchRoll(&y, &p, &r);
	float currentOffset = ((pitchAngleRange - (y - Y_ANGLE_MIN)) / pitchAngleRange) * (maxVerticalOffset - minVerticalOffset) + minVerticalOffset;
	verticalOffsetVector.y = currentOffset;
}

void TCompCameraPlayer::UpdateTargetTransform() {
	TCompTransform* transform = GetTarget()->get<TCompTransform>();
	targetTransform.setPosition(transform->getPosition() + VEC3::Up * 2.f);
	targetTransform.setRotation(transform->getRotation());
}

void TCompCameraPlayer::CenterCamera() {
	if (!isMovementLocked) {
		centeredPosition = targetTransform.getPosition() - targetTransform.getFront() * currentDistanceToTarget;
		VEC3 velocityVector = targetTransform.getPosition() - centeredPosition;
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


TCompTransform* TCompCameraPlayer::GetTransform() {
	return transformHandle;
}