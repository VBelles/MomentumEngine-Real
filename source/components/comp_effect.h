#pragma once

#include "comp_base.h"

struct TMsgEntityCreated;



class TCompEffect : public TCompBase {
	struct EffectEvent {
		float time = 0.f;		// do the action on this time sinze start
		std::string action;		// "start" / "stop"
		bool consumed = false;	// set a true when action is done
	};

	struct SoundEffect {
		std::vector<EffectEvent> events;
		std::string sound;
	};

	struct ParticlesEffect {
		std::vector<EffectEvent> events;
	};



private:
	std::vector<CHandle> soundEffects;
	std::vector<Particles::TParticleHandle> particleSystems;
	std::vector<ParticlesEffect> particleSystems;
	std::vector<SoundEffect> soundSystems;
	CTimer2 timer;
	bool running = false;
public:
	DECL_SIBLING_ACCESS();
	void debugInMenu();
	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	Effect loadEffect(const json & j);
	void onEntityCreated(const TMsgEntityCreated& msg);
	void update(float delta);
	void start();
	void stop();
};
