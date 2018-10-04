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
	if (ImGui::Button("Launch all")) {
		launch();
	}
	if (ImGui::Button("Kill all")) {
		kill();
	}
	int sIndex = 0;
	for (auto& p : systems) {
		auto& system = p.second;
		ImGui::Text("System %d", sIndex);
		ImGui::Text("Resource: %s", system.core->getName().c_str());
		ImGui::Text("Particle handles (%d):", system.particleHandles.size());
		for (auto particleHandle : system.particleHandles) {
			ImGui::Text("\tHandle: %d", particleHandle);
		}
		if (ImGui::Button("Launch")) {
			launch(system.id);
		}
		if (ImGui::Button("Kill")) {
			kill(system.id);
		}
		//ImGui::Text("Particles count: %d", system->getNbParticles());
		sIndex++;
	}
}

void TCompParticles::load(const json& j, TEntityParseContext& ctx) {
	launchOnStart = j.value("launch", launchOnStart);
	fadeOut = j.value("fade_out", 0.f);
	target = j.value("target", "");
	launchConfig.offset = j.count("offset") ? loadVEC3(j["offset"]) : launchConfig.offset;
	launchConfig.rotationOffset = j.count("rotation_offset") ? loadQUAT(j["rotation_offset"]) : launchConfig.rotationOffset;
	launchConfig.bone = j.value("bone", "");
	std::string core = j.value("core", "");
	if (!core.empty()) {
		System system;
		system.core = Resources.get(j.value("core", ""))->as<Particles::TCoreSystem>();
		system.id = core;
		system.target = target;
		system.launchConfig = launchConfig;
		system.fadeOut = fadeOut;
		systems[system.id] = system;
	}
	if (j.count("systems")) {
		for (auto& jSystem : j["systems"]) {
			System system;
			system.id = jSystem.value("id", "");
			system.core = Resources.get(jSystem.value("core", ""))->as<Particles::TCoreSystem>();
			system.target = jSystem.value("target", target);
			system.launchConfig.offset = (jSystem.count("offset") ? loadVEC3(jSystem["offset"]) : system.launchConfig.offset) + launchConfig.offset;
			system.launchConfig.rotationOffset = (jSystem.count("rotation_offset") ? loadQUAT(jSystem["rotation_offset"]) : system.launchConfig.rotationOffset) * launchConfig.rotationOffset;
			system.launchConfig.bone = jSystem.value("bone", launchConfig.bone);
			system.fadeOut = jSystem.value("fade_out", fadeOut);
			systems[system.id] = system;
		}
	}

}

void TCompParticles::onAllScenesCreated(const TMsgAllScenesCreated&) {
	launchConfig.targetEntity = target.empty() ? CHandle(this).getOwner() : getEntityByName(target);
	for (auto& p : systems) {
		auto& system = p.second;
		system.launchConfig.targetEntity = system.target.empty() ? launchConfig.targetEntity : getEntityByName(system.target);
	}
	if (launchOnStart) {
		launch();
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
		systems[id].particleHandles.erase(msg.particleHandle);
		launchedSystems.erase(it);
	}
}

void TCompParticles::launch() {
	CHandle entityHandle = CHandle(this).getOwner();
	for (auto& p : systems) {
		auto& system = p.second;
		auto launchedSystem = EngineParticles.launchSystem(system.core, entityHandle, system.launchConfig);
		system.particleHandles.insert(launchedSystem->getHandle());
		launchedSystems[launchedSystem->getHandle()] = system.id;
	}
}
void TCompParticles::launch(std::string id) {
	CHandle entityHandle = CHandle(this).getOwner();
	auto it = systems.find(id);
	if (it != systems.end()) {
		auto& system = it->second;
		auto launchedSystem = EngineParticles.launchSystem(system.core, entityHandle, system.launchConfig);
		system.particleHandles.insert(launchedSystem->getHandle());
		launchedSystems[launchedSystem->getHandle()] = system.id;
	}
}

void TCompParticles::kill(std::string id) {
	auto it = systems.find(id);
	if (it != systems.end()) {
		auto& system = it->second;
		for (auto particleHandle : system.particleHandles) {
			EngineParticles.kill(particleHandle, system.fadeOut);
		}
	}
}

void TCompParticles::kill() {
	for (auto& p : systems) {
		auto& system = p.second;
		for (auto particleHandle : system.particleHandles) {
			EngineParticles.kill(particleHandle, system.fadeOut);
		}
	}
}

Particles::LaunchConfig TCompParticles::getLaunchConfig(std::string id) {
	auto it = systems.find(id);
	if (it != systems.end()) {
		return it->second.launchConfig;
	}
	return Particles::LaunchConfig();
}

void TCompParticles::setLaunchConfig(std::string id, Particles::LaunchConfig launchConfig) {
	auto it = systems.find(id);
	if (it != systems.end()) {
		it->second.launchConfig = launchConfig;
	}

}

