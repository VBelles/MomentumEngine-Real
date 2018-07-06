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

	Particles::TParticleHandle launchSystem(const std::string& name, CHandle entity, const std::string& bone = "", VEC3 offset = VEC3::Zero, QUAT initialRotation = QUAT::Identity);
	Particles::TParticleHandle launchSystem(const Particles::TCoreSystem* cps, CHandle entity, const std::string& bone = "", VEC3 offset = VEC3::Zero, QUAT initialRotation = QUAT::Identity);

	void kill(Particles::TParticleHandle ph, float fade_out = 0.f);

	void forceEmission(Particles::TParticleHandle ph, int quantity);

	const VEC3& getWindVelocity() const;
};
