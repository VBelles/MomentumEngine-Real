#pragma once

#include "modules/module.h"
#include "entity/msgs.h"


namespace Particles {
	struct TCoreSystem;
	class CSystem;
	using TParticleHandle = int;

	struct LaunchConfig {
		CHandle targetEntity;		//Entity of the transform or bone to follow
		std::string bone;			//Bone to follow
		VEC3 offset = VEC3::Zero;
		QUAT rotationOffset = QUAT::Identity;
	};
}

//Message to notify TCompParticleComponent its system is destroyed
struct TMsgParticleSystemDestroyed {
	Particles::TParticleHandle particleHandle;
	DECL_MSG_ID();
};

class CModuleParticles : public IModule {
private:
	std::unordered_map<std::string, std::vector<Particles::CSystem*>> _activeSystems;
	VEC3                             _windVelocity = VEC3::Zero;
	Particles::TParticleHandle		_lastHandle;
	bool paused = false;
	bool culling = true;
	CHandle cullingHandle;

public:
	CModuleParticles(const std::string& name);
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;
	void reset();

	Particles::TParticleHandle launchSystem(const std::string& name, Particles::LaunchConfig config);
	Particles::CSystem* launchSystem(const Particles::TCoreSystem* cps, CHandle entity, Particles::LaunchConfig config);

	void kill(Particles::TParticleHandle ph, float fadeOutTime = 0.f);

	const VEC3& getWindVelocity() const;

	void setPaused(bool paused);
};
