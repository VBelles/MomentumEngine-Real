#pragma once

struct TMsgAllScenesCreated;

class TCompSky : public TCompBase {
private:

	enum Mode {
		SEQUENTIAL, FIXED, RANDOM
	};

	enum SkyboxType {//poner dependiendo del número de skyboxes que haya en json
		DAY, NIGHT
	};

	struct Skybox {
		const CTexture* texture = nullptr;
		float duration;
	};

	//json
	std::vector<Skybox> skyboxes;
	float fixedLerpTime = 0.f;

	Mode currentMode = SEQUENTIAL;
	float currentLerpTime = 0.f;
	int skyboxIndex = 0;
	int numSkyboxes;

	bool waitingToEnter = false;
	int nextSkybox = 0;
	float nextLerpTime = 0.f;

	CTimer changeTimer;
	CTimer lerpTimer;

public:
	DECL_SIBLING_ACCESS();
	void onAllScenesCreated(const TMsgAllScenesCreated& msg);

	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	void debugInMenu();

	void setSkybox(SkyboxType type, float lerpTime);
	void setToSequential();
	void setToRandom();
};
