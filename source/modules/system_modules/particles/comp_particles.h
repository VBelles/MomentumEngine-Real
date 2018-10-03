#pragma once

#include "modules/system_modules/particles/particle_system.h"

class TCompParticles : public TCompBase {
private:
	struct System {
		std::string id;
		std::string target;
		Particles::LaunchConfig launchConfig;
		float fadeOut = 0.f;
		const Particles::TCoreSystem* core;
		Particles::TParticleHandle particleHandle;
	};

	std::map<std::string, System> systems;						// Map of id -> system
	std::map<Particles::TParticleHandle, std::string> launchedSystems;	// Map of handle -> id

	std::vector<const Particles::TCoreSystem*> cores;

	// Global config
	float fadeOut = 0.f;
	std::string target;
	Particles::LaunchConfig launchConfig;

	//Message callbacks
	void onAllScenesCreated(const TMsgAllScenesCreated&);
	void onDestroyed(const TMsgEntityDestroyed&);
	void onParticleSystemDestroyed(const TMsgParticleSystemDestroyed &);

	void launch();

	void launch(std::string id);

	void kill(std::string id);

	void kill();


public:
	DECL_SIBLING_ACCESS();
	TCompParticles() = default;
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
};
