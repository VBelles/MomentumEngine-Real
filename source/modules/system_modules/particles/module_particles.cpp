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

	_windVelocity = VEC3(0, 0, 0);

	return true;
}

bool CModuleParticles::stop() {
	return true;
}

void CModuleParticles::update(float delta) {
	for (auto it = _activeSystems.begin(); it != _activeSystems.end();) {
		Particles::CSystem* ps = *it;

		bool active = ps->update(delta);
		if (!active) {
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

Particles::TParticleHandle CModuleParticles::launchSystem(
	const std::string& name, CHandle entity) {
	const Particles::TCoreSystem* cps = Resources.get(name)->as<Particles::TCoreSystem>();
	return launchSystem(cps, entity, "", VEC3(0, 0, 0));
}

Particles::TParticleHandle CModuleParticles::launchSystem(
	const std::string& name, CHandle entity, VEC3 offset) {
	const Particles::TCoreSystem* cps = Resources.get(name)->as<Particles::TCoreSystem>();
	return launchSystem(cps, entity, "", offset);
}

Particles::TParticleHandle CModuleParticles::launchSystem(
	const std::string& name, CHandle entity, std::string bone, VEC3 offset) {
	const Particles::TCoreSystem* cps = Resources.get(name)->as<Particles::TCoreSystem>();
	return launchSystem(cps, entity, bone, offset);
}

Particles::TParticleHandle CModuleParticles::launchSystem(
	const Particles::TCoreSystem* cps, CHandle entity) {
	assert(cps);
	Particles::CSystem* ps = new Particles::CSystem(cps, entity);
	ps->launch();
	_activeSystems.push_back(ps);

	return ps->getHandle();
}

Particles::TParticleHandle CModuleParticles::launchSystem(
	const Particles::TCoreSystem* cps, CHandle entity, VEC3 offset) {
	assert(cps);
	Particles::CSystem* ps = new Particles::CSystem(cps, entity, "", offset);
	ps->launch();
	_activeSystems.push_back(ps);

	return ps->getHandle();
}

Particles::TParticleHandle CModuleParticles::launchSystem(
	const Particles::TCoreSystem* cps, CHandle entity, std::string bone, VEC3 offset) {
	assert(cps);
	Particles::CSystem* ps = new Particles::CSystem(cps, entity, bone, offset);
	ps->launch();
	_activeSystems.push_back(ps);

	return ps->getHandle();
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
			delete *it;
			_activeSystems.erase(it);
		}
	}
}

const VEC3& CModuleParticles::getWindVelocity() const {
	return _windVelocity;
}
