#pragma once

#include "components/comp_base.h"
#include "modules/system_modules/particles/particle_system.h"

struct TCompParticles : public TCompBase {
	const Particles::TCoreSystem* _core = nullptr;
	Particles::TParticlesHandle _particles = 0;
	bool _launched = false;
	float _fadeOut = 0.f;
	VEC3 offset = { 0, 0, 0 };

	TCompParticles() = default;
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);

	//Message callbacks
	void onCreated(const TMsgEntityCreated&);
	void onDestroyed(const TMsgEntityDestroyed&);

	void forceEmission(int quantity);
};
