#pragma once

class TCompParticles;
class TCompLightPoint;
class TCompLightFlicker;
class TCompSound;

class TCompFire : public TCompBase {
private:

	std::vector<std::string> fireParticles;
	std::vector<std::string> smokeParticles;
	CHandle particlesHandle;
	CHandle soundHandle;
	CHandle lightHandle;
	CHandle lightFlicker;
	bool hasFire = true;
	bool playerOnFire = false;

	CTimer2 timer;
	float time = 2.f;

	TCompParticles* getParticles();
	TCompSound* getSound();
	TCompLightPoint* getLight();
	TCompLightFlicker* getLightFlicker();
	void onEntityCreated(const TMsgEntityCreated& msg);
	void onPlayerEnter(const TMsgTriggerEnter& msg);
	void onPlayerExit(const TMsgTriggerExit & msg);
public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);
};
