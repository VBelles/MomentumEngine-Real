#pragma once

class TCompTransform;
class TCompLightDir;
struct TMsgAllScenesCreated;

struct TCompSun : public TCompBase {
	DECL_SIBLING_ACCESS();
	
	//json
	VEC3 rotationCenter = VEC3(0.f,0.f,0.f);
	float rotationSpeed;
	float distance;
	float initialYaw;
	float initialPitch;
	float initialRoll;

	float currentYaw;
	float currentPitch;
	float currentRoll;

	CTimer rotationUpdateTimer;
	float rotationUpdateTime = 0.15f;

	float startSunsetYaw = -70.f;
	float endSunsetYaw = -65.f;

	float noonLightIntensity;

	CHandle lightHandle;
	CHandle transformHandle;

	void onAllScenesCreated(const TMsgAllScenesCreated& msg);

	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	void debugInMenu();

	TCompTransform* getTransform() { return transformHandle; }
	TCompLightDir* getLight() { return lightHandle; }
};
