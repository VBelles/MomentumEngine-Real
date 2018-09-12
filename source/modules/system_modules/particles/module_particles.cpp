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
			safeDelete(system);
		}
		systems.clear();
	}
	_activeSystems.clear();
}

void CModuleParticles::update(float delta) {
	if (paused) return;
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

	if (CApp::get().isDebug()) {
		if (ImGui::TreeNode("Particle Systems")) {
			for (auto& p : _activeSystems) {
				auto& systems = p.second;
				for (auto& system : systems) {
					system->debugInMenu();
				}
			}
			ImGui::TreePop();
		}
	}
}

Particles::TParticleHandle CModuleParticles::launchSystem(const std::string& name, Particles::LaunchConfig config) {
	auto cps = Resources.get(name)->as<Particles::TCoreSystem>();
	return launchSystem(cps, CHandle(), config)->getHandle();
}

Particles::CSystem* CModuleParticles::launchSystem(const Particles::TCoreSystem* cps, CHandle entity, Particles::LaunchConfig config) {
	assert(cps);
	auto ps = new Particles::CSystem(++_lastHandle, cps, entity, config);
	ps->launch();
	_activeSystems[cps->render.technique->getName()].push_back(ps);
	return ps;
}

void CModuleParticles::kill(Particles::TParticleHandle ph, float fadeOutTime) {
	for (auto& p : _activeSystems) {
		auto& systems = p.second;
		auto it = std::find_if(systems.begin(), systems.end(), [&ph](const Particles::CSystem* ps) {
			return ps->getHandle() == ph;
		});
		if (it != systems.end()) {
			Particles::CSystem* ps = *it;
			if (fadeOutTime > 0.f) {
				ps->fadeOut(fadeOutTime);
			}
			else {
				if (ps->getParticleEntityHandle().isValid()) { //Send message to entity
					static_cast<CEntity*>(ps->getParticleEntityHandle())->sendMsg(TMsgParticleSystemDestroyed{ ps->getHandle() });
				}
				delete ps;
				systems.erase(it);
			}
		}
	}
}

void CModuleParticles::forceEmission(Particles::TParticleHandle ph, int quantity) {
	for (auto& p : _activeSystems) {
		auto& systems = p.second;
		for (auto system : systems) {
			if (system->getHandle() == ph) {
				system->forceEmission(quantity);
				break;
			}
		}
	}
}

const VEC3& CModuleParticles::getWindVelocity() const {
	return _windVelocity;
}

void CModuleParticles::setPaused(bool paused) {
	this->paused = paused;
}
