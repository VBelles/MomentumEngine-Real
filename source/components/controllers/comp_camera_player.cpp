#include "mcv_platform.h"
#include "comp_camera_player.h"
#include "components/comp_camera.h"
#include "components/comp_transform.h"
#include "components/player/comp_player_model.h"
#include "modules/game_modules/physics/basic_query_filter_callback.h"
#include "game_constants.h"
#include "entity/common_msgs.h"

DECL_OBJ_MANAGER("camera_player", TCompCameraPlayer);

void TCompCameraPlayer::debugInMenu() {
	ImGui::DragFloat("Distance to target", &defaultDistanceToTarget, 0.1f, 1.f, 1000.f);
	ImGui::DragFloat("Offset", &offset, 0.1f, 0.f, 3.f);
	ImGui::DragFloat("Pitch offset", &pitchOffset, 0.1f, -360.f, 360.f);
	ImGui::DragFloat("Zoom in speed", &zoomInSpeed, 0.1f, 0.1f, 20.f);
	ImGui::DragFloat("Zoom out speed", &zoomOutSpeed, 0.1f, 0.1f, 20.f);
	ImGui::DragFloat("Mouse Lerp", &mouseLerpCoef, 0.1f, 0.f, 100000.f);
}

// -------------------------------------------------
void TCompCameraPlayer::renderDebug() {
}

// -------------------------------------------------
void TCompCameraPlayer::load(const json& j, TEntityParseContext& ctx) {
	targetName = j.value("target", "");
	defaultDistanceToTarget = j.value("distance_to_target", 4.5f);
	currentDistanceToTarget = defaultDistanceToTarget;
	runDistanceToTarget = defaultDistanceToTarget;
	idleDistanceToTarget = j.value("idle_distance_to_target", 2.5f);
	cameraSpeed = loadVEC2(j["camera_speed"]);
	zoomOutSpeed = (j.value("zoomOutSpeed", 20.f));
	defaultZoomOutSpeed = zoomOutSpeed;
	zoomInSpeed = (j.value("zoomInSpeed", 10.f));
	defaultZoomInSpeed = zoomInSpeed;
	maxPitch = deg2rad(j.value("max_pitch", 80.f));
	minPitch = deg2rad(j.value("min_pitch", -80.f));
	initialYaw = deg2rad(j.value("yaw", 0.f));
	initialPitch = deg2rad(j.value("pitch", -20.f));
	centeringCameraSpeed = loadVEC2(j["centering_camera_speed"]);
	zoomInSpeedIdleRun = j.value("zoomInSpeedIdleRun", 2.8f);
	zoomOutSpeedIdleRun = j.value("zoomOutSpeedIdleRun", 5.2f);
	minPitchOffset = j.value("minPitchOffset", 4.f);
	maxPitchOffset = j.value("maxPitchOffset", 16.f);
	pitchOffsetThreshold = j.value("pitchOffsetThreshold", 0.f);
	currentCenteringCameraSpeed = centeringCameraSpeed;
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
	CEntity* entity = targetHandle;
	TCompPlayerModel* model = entity->get<TCompPlayerModel>();
	playerVelocityVector = model->getVelocityVector();
	getTransform()->setYawPitchRoll(initialYaw, initialPitch);
	sphereCastRadius = ((TCompCamera*)get<TCompCamera>())->getCamera()->getZNear() * 2;

	prevTargetPosition = getTransform()->getPosition() + VEC3::Up * offset;
}

void TCompCameraPlayer::update(float delta) {
	TCompTransform* transform = getTransform();
	float y, p, r;
	transform->getYawPitchRoll(&y, &p, &r);
	//transform->setYawPitchRoll(y, p - pitchOffset);
	transform->setYawPitchRoll(y, p - deg2rad(pitchOffset));
	updateTargetTransform(delta);
	updateInput();
	if (centeringCamera) {
		updateCenteringCamera(delta);
	}
	else {
		updateRotation(delta);
		updatePosition(delta);

	}

	if (!isMovementLocked && (isCameraInsideGeometry() || sphereCast())) {
		sweepBack();
		zoomInSpeed = defaultZoomInSpeed;
		zoomOutSpeed = defaultZoomOutSpeed;
	}

	transform->getYawPitchRoll(&y, &p, &r);
	//calcular pitchOffset seg�n pitch
	pitchOffset = calculatePitchOffset(p);
	//transform->setYawPitchRoll(y, p + pitchOffset);
	transform->setYawPitchRoll(y, p + deg2rad(pitchOffset));
}

void TCompCameraPlayer::updateTargetTransform(float delta) {
	//Target transform is player transform + offset
	TCompTransform* transform = getTarget()->get<TCompTransform>();
	VEC3 desiredPosition = transform->getPosition() + VEC3::Up * offset;
	float speed = playerVelocityVector->Length();

	float f = lerp(0.00000005f, 0.0000000001f, clamp(speed, 0.f, 20.f)/20.f);
	//const float f = 0.0000001f;
	desiredPosition = VEC3::Lerp(prevTargetPosition, desiredPosition, 1 - pow(f, delta));

	targetTransform.setPosition(desiredPosition);
	targetTransform.setRotation(transform->getRotation());

	prevTargetPosition = targetTransform.getPosition();
}

void TCompCameraPlayer::updateInput() {
	input = VEC2::Zero;
	//Hacer s�lo si la c�mara est� mixeada
	if (!isMovementLocked) {
		VEC2 padInput = VEC2(
			EngineInput[Input::EPadButton::PAD_RANALOG_X].value,
			EngineInput[Input::EPadButton::PAD_RANALOG_Y].value
		);
		if (padInput.Length() > PAD_DEAD_ZONE) {
			input.x -= padInput.x * cameraSpeed.x;
			input.y += padInput.y * cameraSpeed.y;
		}
		else if (!CApp::get().isDebug()) {
			auto& mouse = EngineInput[Input::PLAYER_1].mouse();
			input.x -= mouse.position_delta.x * cameraSpeed.x;
			input.y -= mouse.position_delta.y * cameraSpeed.y;
		}
	}
}

void TCompCameraPlayer::updateRotation(float delta) {
	TCompTransform* transform = getTransform();

	//Move the camera to the target position
	transform->setPosition(targetTransform.getPosition());

	//Rotate the camera
	VEC2 yawPitch;
	transform->getYawPitchRoll(&yawPitch.x, &yawPitch.y);

	VEC2 desiredYawPitch = yawPitch + input * delta;
	desiredYawPitch.y = clamp(desiredYawPitch.y, minPitch, maxPitch);
	
	yawPitch = VEC2::Lerp(yawPitch, desiredYawPitch, 1 - exp(-mouseLerpCoef * delta));
	//yawPitch = desiredYawPitch;
	
	transform->setYawPitchRoll(yawPitch.x, yawPitch.y);
}

void TCompCameraPlayer::updatePosition(float delta) {
	float distance = isDistanceForced ? forcedDistance : defaultDistanceToTarget;
	bool isCloserThanDefault = currentDistanceToTarget < distance ? true : false;
	if (isCloserThanDefault) {
		if (zoomOutSpeed * delta < abs(currentDistanceToTarget - distance)) {
			currentDistanceToTarget = currentDistanceToTarget + zoomOutSpeed * delta;
		}
		else {
			currentDistanceToTarget = distance;
		}
	}
	else {
		if (zoomInSpeed * delta < abs(currentDistanceToTarget - distance)) {
			currentDistanceToTarget = currentDistanceToTarget - zoomInSpeed * delta;
		}
		else {
			currentDistanceToTarget = distance;
		}
	}
	TCompTransform* transform = getTransform();
	VEC3 desiredPos = transform->getPosition() - transform->getFront() * currentDistanceToTarget;
	VEC3 finalPos = desiredPos; //TODO lerepe
	transform->setPosition(finalPos);
}

void TCompCameraPlayer::updateCenteringCamera(float delta) {
	TCompTransform* transform = getTransform();

	//Move the camera to the target position
	transform->setPosition(targetTransform.getPosition());
	float yaw, pitch, r;
	transform->getYawPitchRoll(&yaw, &pitch, &r);

	VEC2 increment = currentCenteringCameraSpeed * delta;

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

	updatePosition(delta);
}


float TCompCameraPlayer::calculatePitchOffset(float pitch) {
	if (pitch > pitchOffsetThreshold) return maxPitchOffset;
	float ratio = abs(pitch - pitchOffsetThreshold) / abs(minPitch - pitchOffsetThreshold);
	float res = lerp(maxPitchOffset, minPitchOffset, ratio);
	return res;
}


void TCompCameraPlayer::centerCamera() {
	if (!isMovementLocked) {
		currentCenteringCameraSpeed = centeringCameraSpeed;
		centeringCamera = true;
		VEC3 front = targetTransform.getFront();
		if (isCenteringCameraForced) {
			float centeringYaw;
			if (isYawSuggested) {
				centeringYaw = suggestedYaw;
				if (hasOppositeYaw) {
					//mirar si nos va mejor ir al opuesto
					float currentYaw, pitch;
					getTransform()->getYawPitchRoll(&currentYaw, &pitch);
					if (abs(suggestedYaw - currentYaw) > M_PI / 2 && abs(suggestedYaw - currentYaw) < M_PI * 1.5f) {
						centeringYaw = suggestedYaw + M_PI;
					}
				}
			}
			else {
				centeringYaw = atan2(front.x, front.z);
			}
			float centeringPitch = isPitchSuggested ? suggestedPitch : initialPitch;
			desiredYawPitch = VEC2(centeringYaw, centeringPitch);
		}
		else {
			desiredYawPitch = VEC2(atan2(front.x, front.z), initialPitch);
		}
	}
}



void TCompCameraPlayer::suggestYawPitchDistance(float yaw, float pitch, float distance, bool suggestYaw, bool hasOppositeYaw, bool suggestPitch, bool forceDistance, bool changeCenteringCamera) {
	suggestedYaw = yaw;
	this->hasOppositeYaw = hasOppositeYaw;
	if (hasOppositeYaw) {
		//mirar si nos va mejor ir al opuesto
		float currentYaw, pitch;
		getTransform()->getYawPitchRoll(&currentYaw, &pitch);
		if (abs(yaw - currentYaw) > M_PI / 2 && abs(yaw - currentYaw) < M_PI * 1.5f) {
			suggestedYaw = yaw + M_PI;
		}
	}
	suggestedPitch = pitch;
	forcedDistance = distance;

	isYawSuggested = suggestYaw;
	isPitchSuggested = suggestPitch;
	isDistanceForced = forceDistance;
	isCenteringCameraForced = changeCenteringCamera;
}

void TCompCameraPlayer::placeCameraOnSuggestedPosition(VEC2 centeringSpeed) {
	if (!isMovementLocked) {
		currentCenteringCameraSpeed = centeringSpeed;
		centeringCamera = true;
		VEC3 front = targetTransform.getFront();
		float currentYaw, currentPitch;
		getTransform()->getYawPitchRoll(&currentYaw, &currentPitch);
		float centeringYaw = isYawSuggested ? suggestedYaw : currentYaw;
		float centeringPitch = isPitchSuggested ? suggestedPitch : currentPitch;
		desiredYawPitch = VEC2(centeringYaw, centeringPitch);
	}
}

void TCompCameraPlayer::resetSuggested() {
	isYawSuggested = false;
	isPitchSuggested = false;
	isDistanceForced = false;
	isCenteringCameraForced = false;
	centeringCamera = false;
}

void TCompCameraPlayer::lockCameraInput(bool isLocked) {
	isMovementLocked = isLocked;
	centeringCamera = false; //Avoid bugs
}

void TCompCameraPlayer::moveCameraCloser(bool wantClose) {
	if (!isDistanceForced) {
		zoomInSpeed = zoomInSpeedIdleRun;
		zoomOutSpeed = zoomOutSpeedIdleRun;
		if (wantClose) {
			defaultDistanceToTarget = idleDistanceToTarget;
		}
		else {
			defaultDistanceToTarget = runDistanceToTarget;
		}
	}
}

void TCompCameraPlayer::sweepBack() {
	TCompTransform* transform = getTransform();
	VEC3 targetPosition = targetTransform.getPosition();
	VEC3 position = transform->getPosition();
	QUAT rotation = transform->getRotation();
	VEC3 direction = position - targetPosition;
	direction.Normalize();
	PxSphereGeometry geometry(sphereCastRadius);
	PxReal distance;
	distance = isDistanceForced ? forcedDistance : defaultDistanceToTarget;
	PxSweepBuffer sweepBuffer;
	PxQueryFilterData fd;
	fd.data = PxFilterData(EnginePhysics.Player, EnginePhysics.Scenario, 0, 0);
	fd.flags |= PxQueryFlag::ePREFILTER;
	PxHitFlags hitFlags = PxHitFlag::eDEFAULT;

	bool status = EnginePhysics.getScene()->sweep(geometry, PxTransform(toPxVec3(targetPosition), toPxQuat(rotation)), toPxVec3(direction), distance, sweepBuffer,
		hitFlags, fd, EnginePhysics.getGameQueryFilterCallback());
	//Si empiezas el sweep fuera de la c�psula del player la c�mara har� el loco si all� hay un collider...
	if (status) {
		PxSweepHit& hit = sweepBuffer.block;
		PxVec3 newPosition = hit.position + (hit.normal * sphereCastRadius);
		transform->setPosition(toVec3(newPosition));
		currentDistanceToTarget = VEC3::Distance(transform->getPosition(), targetTransform.getPosition());
		//dbg("%f, %f, %f\n", newPosition.x, newPosition.y, newPosition.z);
		//dbg("Distance: %f\n", currentDistanceToTarget);

	}
	else {
		currentDistanceToTarget = distance;
	}
	//dbg("Status: %d\n", status);
}

bool TCompCameraPlayer::sphereCast() {
	PxSphereGeometry sphereShape(sphereCastRadius); //shape to test for overlaps
	PxTransform pxTransform = toPxTransform(getTransform());
	PxOverlapBuffer buf;
	PxQueryFilterData fd;
	fd.data = PxFilterData(EnginePhysics.Player, EnginePhysics.Scenario, 0, 0);
	fd.flags |= PxQueryFlag::eANY_HIT | PxQueryFlag::ePREFILTER;
	bool status = EnginePhysics.getScene()->overlap(sphereShape, pxTransform, buf, fd, EnginePhysics.getGameQueryFilterCallback());
	return status;
}

bool TCompCameraPlayer::isCameraInsideGeometry() {
	VEC3 cameraPosition = getTransform()->getPosition();
	VEC3 targetPosition = targetTransform.getPosition();

	const PxU32 bufferSize = 256;        // [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];  // [out] User provided buffer for results
	PxRaycastBuffer buf1(hitBuffer, bufferSize); // [out] Blocking and touching hits stored here
	raycast(targetPosition, cameraPosition, buf1);
	PxRaycastHit hitBuffer2[bufferSize];  // [out] User provided buffer for results
	PxRaycastBuffer buf2(hitBuffer2, bufferSize); // [out] Blocking and touching hits stored here
	raycast(cameraPosition, targetPosition, buf2);

	//dbg("Buf1: %d\n", buf1.nbTouches);
	//dbg("Buf2: %d\n", buf2.nbTouches);

	return buf1.nbTouches != buf2.nbTouches;
}

bool TCompCameraPlayer::raycast(VEC3 origin, VEC3 destination, PxRaycastCallback& callback) {
	VEC3 dir = destination - origin;
	dir.Normalize();
	float distance = VEC3::Distance(origin, destination);
	PxHitFlags hitflags = PxHitFlags(PxHitFlag::eDEFAULT);
	PxQueryFilterData filterData(PxFilterData(EnginePhysics.Player, EnginePhysics.Scenario, 0, 0),
		PxQueryFlag::eDYNAMIC | PxQueryFlag::eSTATIC | PxQueryFlag::ePREFILTER | PxQueryFlag::eNO_BLOCK);
	bool status = EnginePhysics.getScene()->raycast(toPxVec3(origin), toPxVec3(dir), distance, callback, hitflags, filterData, EnginePhysics.getGameQueryFilterCallback());
	return status;
}
