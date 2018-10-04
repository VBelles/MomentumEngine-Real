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
		std::set< Particles::TParticleHandle> particleHandles;
	};

	std::unordered_map<std::string, System> systems;								// Map of id -> system
	std::unordered_map<Particles::TParticleHandle, std::string> launchedSystems;	// Map of handle -> id

	// Global config
	float fadeOut = 0.f;
	std::string target;
	Particles::LaunchConfig launchConfig;
	bool launchOnStart = true;

	//Message callbacks
	void onAllScenesCreated(const TMsgAllScenesCreated&);
	void onDestroyed(const TMsgEntityDestroyed&);
	void onParticleSystemDestroyed(const TMsgParticleSystemDestroyed &);




public:
	DECL_SIBLING_ACCESS();
	TCompParticles() = default;
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);

	void launch();
	void launch(std::string id);
	void kill(std::string id);
	void kill();

	Particles::LaunchConfig getLaunchConfig(std::string id);
	void setLaunchConfig(std::string id, Particles::LaunchConfig launchConfig);
};
