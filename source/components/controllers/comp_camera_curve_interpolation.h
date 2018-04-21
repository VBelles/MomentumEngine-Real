#pragma once

#include "components/comp_base.h"

class CHandle;
class TCompTransform;
class TCompCamera;

struct TMsgEntitiesGroupCreated;

class TCompCameraCurveInterpolation : public TCompBase {
	DECL_SIBLING_ACCESS();

private:
	
	CHandle transformHandle;
	CHandle cameraHandle;
	CHandle cameraToBlend;

	bool isInterpolating = false;

	float weight = 0.f;
	float time = 0.f;
	float timeToBlend;

	VEC3 endingPos;
	VEC3 endingFront;
	float endingYaw;
	float endingPitch;
	float endingFov;
	float endingZnear;
	float endingZfar;

	VEC3 startingPos;
	VEC3 startingFront;
	float startingYaw;
	float startingPitch;
	float startingFov;
	float startingZnear;
	float startingZfar;

	VEC3 orbitCenter;
	float currentDistanceToCenter;
	float maxDistanceToCenter;//max -> 180 grados de diferencia en yaw, 0 -> 90 grados
	float halfwayDistanceToCenter;
	VEC3 orbitFront;
	VEC3 startingOrbitFront;

	Interpolator::IInterpolator* interpolator = nullptr;

	//Msgs
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);


	TCompTransform* getTransform() { return transformHandle; }
	TCompCamera* getCamera() { return cameraHandle; }

public:
	static void registerMsgs();
	void debugInMenu();
	void renderDebug();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	//Se calcula la velocidad angular a la que tiene que ir
	void startInterpolating(
		CHandle cameraToBlend, float timeToBlend, VEC3 endingPos, VEC3 endingFront, float endingFov, 
		float endingZnear, float endingZfar, Interpolator::IInterpolator* newInterpolator = nullptr);
	void stopInterpolating();
};

