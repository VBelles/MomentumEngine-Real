#pragma once

class TCompTransform;
class TCompLightDir;
struct TMsgAllScenesCreated;

class TCompSky : public TCompBase {
private:
	struct Skybox {
		const CTexture* texture = nullptr;
		float duration;
	};

	//json
	std::vector<Skybox> skyboxes;
	float fixedLerpTime = 0.f;

	float currentLerpTime = 0.f;
	int skyboxIndex = 0;
	int numSkyboxes;
	CTimer changeTimer;
	CTimer lerpTimer;
public:
	DECL_SIBLING_ACCESS();
	void onAllScenesCreated(const TMsgAllScenesCreated& msg);

	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	void debugInMenu();

};
