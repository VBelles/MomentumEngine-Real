#include "mcv_platform.h"
#include "comp_particles.h"
#include "resources/resources_manager.h"

DECL_OBJ_MANAGER("particles", TCompParticles);

void TCompParticles::registerMsgs() {
	DECL_MSG(TCompParticles, TMsgAllScenesCreated, onAllScenesCreated);
	DECL_MSG(TCompParticles, TMsgEntityDestroyed, onDestroyed);
	DECL_MSG(TCompParticles, TMsgParticleSystemDestroyed, onParticleSystemDestroyed);
}

void TCompParticles::debugInMenu() {
	ImGui::Text("Systems: %d", systems.size());
	int sIndex = 0;
	for (auto p : systems) {
		auto& system = p.second;
		ImGui::Text("System %d", sIndex);
		ImGui::Text("Resource: %s", system->getCore()->getName().c_str());
		ImGui::Text("Particles handle: %d", system->getHandle());
		ImGui::Text("Particles count: %d", system->getNbParticles());
		sIndex++;
	}
}

void TCompParticles::load(const json& j, TEntityParseContext& ctx) {
	_fadeOut = j.value("fade_out", 0.f);
	for (std::string core : j["core"]) {
		cores.push_back(Resources.get(core)->as<Particles::TCoreSystem>());
	}
	target = j.value("target", "");
	launchConfig.offset = j.count("offset") ? loadVEC3(j["offset"]) : launchConfig.offset;
	launchConfig.rotationOffset = j.count("rotation_offset") ? loadQUAT(j["rotation_offset"]) : launchConfig.rotationOffset;
	launchConfig.bone = j.value("bone", "");
	assert(_core);
}

void TCompParticles::onAllScenesCreated(const TMsgAllScenesCreated&) {
	launchConfig.targetEntity = target.empty() ? CHandle(this).getOwner() : getEntityByName(target);
	CHandle entityHandle = CHandle(this).getOwner();
	for (auto core : cores) {
		Particles::CSystem* system = EngineParticles.launchSystem(core, entityHandle, launchConfig);
		systems[system->getHandle()] = system;
	}
	_launched = true;
}

void TCompParticles::onDestroyed(const TMsgEntityDestroyed&) {
	for (auto p : systems) {
		EngineParticles.kill(p.second->getHandle(), _fadeOut);
	}
	systems.clear();
}

void TCompParticles::onParticleSystemDestroyed(const TMsgParticleSystemDestroyed& msg) {
	msg.particleHandle;
	systems.clear();
}
