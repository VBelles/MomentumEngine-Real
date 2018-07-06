#include "mcv_platform.h"
#include "comp_particles.h"
#include "resources/resources_manager.h"

DECL_OBJ_MANAGER("particles", TCompParticles);

void TCompParticles::registerMsgs() {
    DECL_MSG(TCompParticles, TMsgEntityCreated, onCreated);
    DECL_MSG(TCompParticles, TMsgEntityDestroyed, onDestroyed);
}

void TCompParticles::debugInMenu() {
    if (_core) {
        ImGui::Text("Core: %s", _core->getName().c_str());
    }
    ImGui::LabelText("Particles handle", "%d", _particles);
}

void TCompParticles::load(const json& j, TEntityParseContext& ctx) {
    _fadeOut = j.value("fade_out", 0.f);
    auto& particlesName = j.value("core", "");
    _core = Resources.get(particlesName)->as<Particles::TCoreSystem>();
	if (j.count("offset")) {
		offset = loadVEC3(j["offset"]);
	}
    assert(_core);
}

void TCompParticles::onCreated(const TMsgEntityCreated&) {
    if (_core && !_particles) {
        _particles = EngineParticles.launchSystem(_core, CHandle(this).getOwner(), offset);
    }
}

void TCompParticles::onDestroyed(const TMsgEntityDestroyed&) {
    if (_particles) {
        EngineParticles.kill(_particles, _fadeOut);
    }
}

void TCompParticles::forceEmission(int quantity) {
	if (_particles) {
		EngineParticles.forceEmission(_particles, quantity);
	}
}
