#pragma once

#include "modules/system_modules/particles/particle_system.h"

class TCompParticles : public TCompBase {
private:
	std::vector<const Particles::TCoreSystem*> cores;
	bool _launched = false;
	float _fadeOut = 0.f;
	std::string target;

	std::map<int, Particles::CSystem*> systems;

	Particles::LaunchConfig launchConfig;

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
};
