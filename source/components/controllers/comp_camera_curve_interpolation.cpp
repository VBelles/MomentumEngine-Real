#include "mcv_platform.h"
#include "comp_camera_player.h"
#include "components/comp_camera.h"
#include "comp_camera_curve_interpolation.h"
#include "components/comp_transform.h"
#include "game_constants.h"
#include "entity/common_msgs.h"
#include "geometry/SimpleMath.h"

DECL_OBJ_MANAGER("camera_curve_interpolation", TCompCameraCurveInterpolation);

void TCompCameraCurveInterpolation::debugInMenu() {
}

// -------------------------------------------------
void TCompCameraCurveInterpolation::renderDebug() {
}

// -------------------------------------------------
void TCompCameraCurveInterpolation::load(const json& j, TEntityParseContext& ctx) {
}

void TCompCameraCurveInterpolation::registerMsgs() {
	DECL_MSG(TCompCameraCurveInterpolation, TMsgEntitiesGroupCreated, onGroupCreated);
}


void TCompCameraCurveInterpolation::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	transformHandle = get<TCompTransform>();
	cameraHandle = get<TCompCamera>();
}

void TCompCameraCurveInterpolation::orbit(float ratio) {
	TCompTransform* transform = getTransform();
	transform->setPosition(transform->getPosition() + (orbitFront * currentDistanceToCenter));
	//Lerp orbitFront and distance
	//orbitFront = VEC3::Lerp(startingOrbitFront, -startingOrbitFront, ratio);//Cómo sabe en qué sentido rotar
	float orbitYaw = lerp(startingOrbitYaw, endingOrbitYaw, weight);//lo queremos lineal?
	orbitFront = VEC3(sin(orbitYaw), 0.f, cos(orbitYaw));
	if (ratio < 0.5f) {
		currentDistanceToCenter = lerp(maxDistanceToCenter, halfwayDistanceToCenter, ratio * 2);
	}
	else {
		currentDistanceToCenter = lerp(halfwayDistanceToCenter, maxDistanceToCenter, 1.f - ratio * 2);
	}
	height = lerp(startingPos.y, endingPos.y, ratio);
	VEC3 newPos = transform->getPosition() - (orbitFront * currentDistanceToCenter);
	newPos.y = height;
	transform->setPosition(newPos);
}

void TCompCameraCurveInterpolation::update(float delta) {
	if (isInterpolating) {
		time += delta;
		weight = clamp(time / timeToBlend, 0.f, 1.f);
		float ratio = weight;
		if (interpolator) {
			ratio = interpolator->blend(0.f, 1.f, ratio);
		}
		
		TCompTransform* transform = getTransform();
		CCamera* camera = ((TCompCamera*)getCamera())->getCamera();

		orbit(ratio);

		//Lerp de Front
		VEC3 newFront = VEC3::Lerp(startingFront, endingFront, weight);//interpolamos front linealmente
		float newYaw, newPitch;
		getYawPitchFromVector(newFront, &newYaw, &newPitch);
		transform->setYawPitchRoll(newYaw, newPitch);

		float newFov = lerp(startingFov, endingFov, ratio);
		float newZnear = lerp(startingZnear, endingZnear, ratio);
		float newZfar = lerp(startingZfar, endingZfar, ratio);
		
		camera->setPerspective(newFov, newZnear, newZfar);
		camera->lookAt(transform->getPosition(), transform->getPosition() + transform->getFront());

		if (weight == 1.f) {
			isInterpolating = false;
			//mixear siguiente cámara
			Engine.getCameras().blendInCamera(cameraToBlend, 0.001f, CModuleCameras::EPriority::GAMEPLAY);
			return;
		}
	}
}

void TCompCameraCurveInterpolation::startInterpolating(
		CHandle cameraToBlend, float timeToBlend, VEC3 endingPos, VEC3 endingFront, float endingFov, 
		float endingZnear, float endingZfar, Interpolator::IInterpolator * newInterpolator
		) {
	weight = 0.f;
	time = 0.f;
	this->cameraToBlend = cameraToBlend;

	this->timeToBlend = timeToBlend;
	this->endingPos = endingPos;
	this->endingFront = endingFront;
	//yaw y pitch no se necesitan, creo
	getYawPitchFromVector(endingFront, &endingYaw, &endingPitch);
	this->endingFov = endingFov;
	this->endingZnear = endingZnear;
	this->endingZfar = endingZfar;
	interpolator = newInterpolator;

	TCompTransform* transform = getTransform();
	CCamera* camera = ((TCompCamera*)getCamera())->getCamera();
	startingPos = transform->getPosition();
	startingFront = transform->getFront();
	//yaw y pitch no se necesitan, creo
	transform->getYawPitchRoll(&startingYaw, &startingPitch);
	startingFov = camera->getFov();
	startingZfar = camera->getZFar();
	startingZnear = camera->getZNear();

	/*VEC3 halfDistanceVector = (endingPos - startingPos) / 2;
	startingOrbitFront = halfDistanceVector;
	startingOrbitFront.Normalize();
	orbitFront = startingOrbitFront;
	orbitCenter = startingPos + halfDistanceVector;
	maxDistanceToCenter = VEC3::Distance(startingPos, endingPos) / 2;
	currentDistanceToCenter = maxDistanceToCenter;*/

	//Ahora sudando de la y
	VEC3 halfDistanceVector = (endingPos - startingPos) / 2;
	halfDistanceVector.y = 0.f;
	startingOrbitFront = halfDistanceVector;
	startingOrbitFront.Normalize();
	orbitFront = startingOrbitFront;
	maxDistanceToCenter = halfDistanceVector.Length();
	currentDistanceToCenter = maxDistanceToCenter;
	startingOrbitYaw = getYawFromVector(orbitFront);
	//endingOrbitYaw será startingOrbitYaw +- PI según startingYaw y endingYaw
	float yawDifference = endingYaw - startingYaw;
	int sign = 1;
	if (abs(yawDifference) < M_PI) {
		sign = yawDifference < 0 ? -1 : 1;
	}
	else {
		sign = yawDifference < 0 ? 1 : -1;
	}
	endingOrbitYaw = startingOrbitYaw + M_PI * sign;

	VEC2 startingFront2D = VEC2(startingFront.x, startingFront.z);
	VEC2 endingFront2D = VEC2(endingFront.x, endingFront.z);
	float dotBetweenYaws = startingFront2D.Dot(endingFront2D);
	halfwayDistanceToCenter = /*-dotBetweenYaws **/ maxDistanceToCenter;

	isInterpolating = true;
}

void TCompCameraCurveInterpolation::stopInterpolating() {
	isInterpolating = false;
}

//float ratio = mc.weight;
//if (mc.interpolator)
//ratio = mc.interpolator->blend(0.f, 1.f, ratio);

//mc.time += delta;
//	mc.weight = clamp(mc.time / mc.blendInTime, 0.f, 1.f);


//VEC3 newPos = VEC3::Lerp(camera1->getPosition(), camera2->getPosition(), ratio);
//VEC3 newFront = VEC3::Lerp(camera1->getFront(), camera2->getFront(), ratio);

//float newFov = lerp(startingFov, endingFov, ratio);
//float newZnear = lerp(camera1->getZNear(), camera2->getZNear(), ratio);
//float newZfar = lerp(camera1->getZFar(), camera2->getZFar(), ratio);
//
//camera->setPerspective(newFov, newZnear, newZfar);
//camera->lookAt(newPos, newPos + newFront);