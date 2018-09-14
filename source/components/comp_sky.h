#pragma once

struct TMsgAllScenesCreated;

class TCompSky : public TCompBase {
private:

	enum Mode {
		SEQUENTIAL, FIXED, RANDOM
	};

	enum SkyboxType {//poner dependiendo del número de skyboxes que haya en json
		REGULAR, INVERTED, PINK, RED, TURQUESA
	};

	struct Skybox {
		const CTexture* texture = nullptr;
		float duration;
		VEC4 lightColor;
		float lightIntensity;
		float emissiveRatio;
	};

	/*
	REGULAR		1 0.87 0.67 1
	INVERTED	0.033 0334 0.353 1
	PINK		0.912 0.401 0.313 1
	RED			0.603 0.157 0.08 1
	TURQUESA	0.638 0.711 0.704 1
	*/

	//json
	std::vector<Skybox> skyboxes;
	float fixedLerpTime = 0.f;
	std::string lightSourceName;

	CHandle lightSource;

	Mode currentMode = FIXED;
	float currentLerpTime = 0.f;

	size_t skyboxIndex = 0;
	size_t previousSkyboxIndex = 0;
	size_t numSkyboxes;

	bool waitingToEnter = false;
	size_t nextSkybox = 0;
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
