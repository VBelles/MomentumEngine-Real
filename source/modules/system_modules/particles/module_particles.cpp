#include "mcv_platform.h"
#include "module_particles.h"
#include "modules/system_modules/particles/particle_system.h"
#include "modules/system_modules/particles/particle_parser.h"

CModuleParticles::CModuleParticles(const std::string& name)
	: IModule(name)
	, _lastHandle(0) {
}

bool CModuleParticles::start() {
	Resources.registerResourceClass(getResourceClassOf<Particles::TCoreSystem>());
	return true;
}

bool CModuleParticles::stop() {
	reset();
	return true;
}

void CModuleParticles::reset() {
	_lastHandle = 0;

	for (auto& p : _activeSystems) {
		auto& systems = p.second;
		for (auto system : systems) {
			SAFE_DELETE(system);
		}
		systems.clear();
	}
	_activeSystems.clear();
}

void CModuleParticles::update(float delta) {
	for (auto& p : _activeSystems) {
		auto& systems = p.second;
		for (auto it = systems.begin(); it != systems.end();) {
			Particles::CSystem* ps = *it;

			bool active = ps->update(delta);
			if (!active) {
				if (ps->getParticleEntityHandle().isValid()) {
					static_cast<CEntity*>(ps->getParticleEntityHandle())->sendMsg(TMsgParticleSystemDestroyed{ ps->getHandle() });
				}
				delete ps;
				it = systems.erase(it);
			}
			else {
				++it;
			}
		}
	}
}

void CModuleParticles::render() {
	for (auto& p : _activeSystems) {
		auto& systems = p.second;
		for (auto& system : systems) {
			system->render();
		}
	}
}

Particles::ParticleHandle CModuleParticles::launchSystem(const std::string& name, Particles::LaunchConfig config) {
	auto cps = Resources.get(name)->as<Particles::TCoreSystem>();
	return launchSystem(cps, CHandle(), config)->getHandle();
}

Particles::CSystem* CModuleParticles::launchSystem(const Particles::TCoreSystem* cps, CHandle entity, Particles::LaunchConfig config) {
	assert(cps);
	Particles::ParticleHandle handle{ cps->render.technique->getName(), ++_lastHandle };
	auto ps = new Particles::CSystem(handle, cps, entity, config);
	ps->launch();
	_activeSystems[handle.key].push_back(ps);
	return ps;
}

void CModuleParticles::kill(Particles::ParticleHandle ph, float fadeOutTime) {
	auto& systems = _activeSystems[ph.key];
	auto it = std::find_if(systems.begin(),
		systems.end(),
		[&ph](const Particles::CSystem* ps) {
		return ps->getHandle().handle == ph.handle;
	});

	if (it != systems.end()) {
		if (fadeOutTime > 0.f) {
			(*it)->fadeOut(fadeOutTime);
		}
		else {
			if ((*it)->getParticleEntityHandle().isValid()) {
				static_cast<CEntity*>((*it)->getParticleEntityHandle())->sendMsg(TMsgParticleSystemDestroyed{ (*it)->getHandle() });
			}
			delete *it;
			systems.erase(it);
		}
	}
}

void CModuleParticles::forceEmission(Particles::ParticleHandle ph, int quantity) {
	auto systems = _activeSystems[ph.key];
	for (auto system : systems) {
		if (system->getHandle().handle == ph.handle) {
			system->forceEmission(quantity);
			break;
		}
	}
}

const VEC3& CModuleParticles::getWindVelocity() const {
	return _windVelocity;
}
