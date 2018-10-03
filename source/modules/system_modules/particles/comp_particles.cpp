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
	fadeOut = j.value("fade_out", 0.f);
	target = j.value("target", "");
	launchConfig.offset = j.count("offset") ? loadVEC3(j["offset"]) : launchConfig.offset;
	launchConfig.rotationOffset = j.count("rotation_offset") ? loadQUAT(j["rotation_offset"]) : launchConfig.rotationOffset;
	launchConfig.bone = j.value("bone", "");
	for (auto& jSystem : j["system"]) {
		System system;
		system.id = jSystem.value("id", "");
		system.target = jSystem.value("target", target);
		system.launchConfig.offset = jSystem.count("offset") ? loadVEC3(jSystem["offset"]) : system.launchConfig.offset + launchConfig.offset;
		system.launchConfig.rotationOffset = jSystem.count("rotation_offset") ? loadQUAT(jSystem["rotation_offset"]) : system.launchConfig.rotationOffset * launchConfig.rotationOffset;
		system.launchConfig.bone = jSystem.value("bone", launchConfig.bone);
		system.fadeOut = jSystem.value("fade_out", fadeOut);
	}

}

void TCompParticles::onAllScenesCreated(const TMsgAllScenesCreated&) {
	launchConfig.targetEntity = target.empty() ? CHandle(this).getOwner() : getEntityByName(target);
	CHandle entityHandle = CHandle(this).getOwner();
	for (auto core : cores) {
		Particles::CSystem* system = EngineParticles.launchSystem(core, entityHandle, launchConfig);
		systems[system->getHandle()] = system;
	}

}

void TCompParticles::onDestroyed(const TMsgEntityDestroyed& msg) {
	// TODO Seguir investigando este f�nomone: cuando se envia el mensaje TMsgEntityDestroyed el componente ya esta destruido
	/*for (auto p : systems) {
		EngineParticles.kill(p.second->getHandle(), _fadeOut);
	}
	systems.clear();*/
}

void TCompParticles::onParticleSystemDestroyed(const TMsgParticleSystemDestroyed& msg) {
	auto it = launchedSystems.find(msg.particleHandle);
	if (it != launchedSystems.end()) {
		std::string id = launchedSystems[msg.particleHandle];
		systems[id].particleHandle = -1;
		launchedSystems.erase(it);
	}
}

void TCompParticles::launch() {
	CHandle entityHandle = CHandle(this).getOwner();
	for (auto core : cores) {
		EngineParticles.launchSystem(core, entityHandle, Particles::LaunchConfig{});
	}
}
void TCompParticles::launch(std::string id) {
	CHandle entityHandle = CHandle(this).getOwner();
	auto it = systems.find(id);
	if (it != systems.end()) {
		auto& system = it->second;
		auto launchedSystem = EngineParticles.launchSystem(system.core, entityHandle, system.launchConfig);
		launchedSystems[launchedSystem->getHandle()] = system.id;
	}
}

void TCompParticles::kill(std::string id) {
	auto it = systems.find(id);
	if (it != systems.end()) {
		auto& system = it->second;
		EngineParticles.kill(system.particleHandle, system.fadeOut);
	}
}

void TCompParticles::kill() {
	for (auto& p : systems) {
		auto& system = p.second;
		EngineParticles.kill(system.particleHandle, system.fadeOut);
	}
}

