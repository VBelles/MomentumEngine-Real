#pragma once

#include "components/comp_base.h"
#include "modules/system_modules/particles/particle_system.h"

class TCompParticles : public TCompBase {
private:
	const Particles::TCoreSystem* _core = nullptr;
	Particles::CSystem* _particles = nullptr;
	bool _launched = false;
	float _fadeOut = 0.f;
	std::string target;

	Particles::LaunchConfig config;

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
	void forceEmission(int quantity);
	void setOffset(VEC3 offset);
	void setRotationOffset(QUAT rotationOffset);
};
