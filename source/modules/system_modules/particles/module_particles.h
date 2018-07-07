#pragma once

#include "modules/module.h"
#include "entity/msgs.h"





namespace Particles {
	struct TCoreSystem;
	class CSystem;
	using TParticleHandle = int;
}

//Message to notify TCompParticleComponent its system is destroyed
struct TMsgParticleSystemDestroyed {
	Particles::TParticleHandle particleHandle = 0;
	DECL_MSG_ID();
};

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

	Particles::TParticleHandle launchSystem(const std::string& name, CHandle targetEntity, const std::string& bone = "", VEC3 offset = VEC3::Zero, QUAT initialRotation = QUAT::Identity);
	Particles::TParticleHandle launchSystem(const Particles::TCoreSystem* cps, CHandle targetEntity, const std::string& bone = "", VEC3 offset = VEC3::Zero, QUAT initialRotation = QUAT::Identity);
	Particles::CSystem* launchSystemFromComponent(const Particles::TCoreSystem * cps, CHandle particleComponent, CHandle targetEntity, const std::string & bone, VEC3 offset, QUAT rotationOffset);

	void kill(Particles::TParticleHandle ph, float fade_out = 0.f);

	void forceEmission(Particles::TParticleHandle ph, int quantity);

	const VEC3& getWindVelocity() const;
};
