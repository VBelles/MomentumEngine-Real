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

	for (Particles::CSystem* system : _activeSystems) {
		SAFE_DELETE(system);
	}
	_activeSystems.clear();
}

void CModuleParticles::update(float delta) {
	for (auto it = _activeSystems.begin(); it != _activeSystems.end();) {
		Particles::CSystem* ps = *it;

		bool active = ps->update(delta);
		if (!active) {
			if (ps->getParticleEntityHandle().isValid()) {
				static_cast<CEntity*>(ps->getParticleEntityHandle())->sendMsg(TMsgParticleSystemDestroyed{ ps->getHandle() });
			}
			delete ps;
			it = _activeSystems.erase(it);
		}
		else {
			++it;
		}
	}
}

void CModuleParticles::render() {
	for (auto& ps : _activeSystems) {
		ps->render();
	};
}

Particles::TParticleHandle CModuleParticles::launchSystem(const std::string& name, CHandle targetEntity, const std::string& bone, VEC3 offset, QUAT rotationOffset) {
	auto cps = Resources.get(name)->as<Particles::TCoreSystem>();
	return launchSystem(cps, targetEntity, bone, offset, rotationOffset);
}

Particles::TParticleHandle CModuleParticles::launchSystem(const Particles::TCoreSystem* cps, CHandle targetEntity, const std::string& bone, VEC3 offset, QUAT rotationOffset) {
	assert(cps);
	auto ps = new Particles::CSystem(cps, CHandle(), targetEntity, bone, offset, rotationOffset);
	ps->launch();
	_activeSystems.push_back(ps);
	return ps->getHandle();
}

Particles::CSystem* CModuleParticles::launchSystemFromComponent(const Particles::TCoreSystem* cps, CHandle particleComponent, CHandle targetEntity, const std::string& bone, VEC3 offset, QUAT rotationOffset) {
	dbg("Launching system from component\n");
	assert(cps);
	assert(particleComponent.isValid());
	auto ps = new Particles::CSystem(cps, particleComponent.getOwner(), targetEntity, bone, offset, rotationOffset);
	ps->launch();
	_activeSystems.push_back(ps);
	return ps;
}

void CModuleParticles::kill(Particles::TParticleHandle ph, float fadeOutTime) {
	auto it = std::find_if(_activeSystems.begin(),
		_activeSystems.end(),
		[&ph](const Particles::CSystem* ps) {
		return ps->getHandle() == ph;
	});

	if (it != _activeSystems.end()) {
		if (fadeOutTime > 0.f) {
			(*it)->fadeOut(fadeOutTime);
		}
		else {
			if ((*it)->getParticleEntityHandle().isValid()) {
				static_cast<CEntity*>((*it)->getParticleEntityHandle())->sendMsg(TMsgParticleSystemDestroyed{ (*it)->getHandle() });
			}
			delete *it;
			_activeSystems.erase(it);
		}
	}
}

void CModuleParticles::forceEmission(Particles::TParticleHandle ph, int quantity) {
	for (auto system : _activeSystems) {
		if (system->getHandle() == ph) {
			system->forceEmission(quantity);
			break;
		}
	}
}

const VEC3& CModuleParticles::getWindVelocity() const {
	return _windVelocity;
}
