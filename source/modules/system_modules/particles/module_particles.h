#pragma once

#include "modules/module.h"

namespace Particles {
	struct TCoreSystem;
	class CSystem;
	using TParticleHandle = int;
}

class CModuleParticles : public IModule {
private:
	std::vector<Particles::CSystem*> _activeSystems;
	VEC3                             _windVelocity = VEC3::Zero;
	Particles::TParticleHandle       _lastHandle;

public:
	CModuleParticles(const std::string& name);
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;
	void reset();

	Particles::TParticleHandle launchSystem(const std::string& name, VEC3 offset = VEC3::Zero, CHandle entity = CHandle(), const std::string& bone = "");
	Particles::TParticleHandle launchSystem(const Particles::TCoreSystem* cps, VEC3 offset = VEC3::Zero, CHandle entity = CHandle(), const std::string& bone = "");

	void kill(Particles::TParticleHandle ph, float fade_out = 0.f);

	void forceEmission(Particles::TParticleHandle ph, int quantity);

	const VEC3& getWindVelocity() const;
};
