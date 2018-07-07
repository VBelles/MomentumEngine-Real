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
    if (_core) {
        ImGui::Text("Core: %s", _core->getName().c_str());
    }
    ImGui::LabelText("Particles handle", "%d", _particles->getHandle());
}

void TCompParticles::load(const json& j, TEntityParseContext& ctx) {
    _fadeOut = j.value("fade_out", 0.f);
    auto& particlesName = j.value("core", "");
    _core = Resources.get(particlesName)->as<Particles::TCoreSystem>();
	offset = j.count("offset") ? loadVEC3(j["offset"]) : offset;
	rotationOffset = j.count("rotation_offset") ? loadQUAT(j["rotation_offset"]) : rotationOffset;
	target = j.value("target", "");
	bone = j.value("bone", "");
    assert(_core);
}

void TCompParticles::onAllScenesCreated(const TMsgAllScenesCreated&) {
    if (_core && !_particles) {
		CHandle targetEntity;
		if (!target.empty()) {
			targetEntity = getEntityByName(target);
		}
		else {
			targetEntity = CHandle(this).getOwner();
		}
        _particles = EngineParticles.launchSystemFromComponent(_core, CHandle(this), targetEntity, bone, offset, rotationOffset);
    }
}


void TCompParticles::onDestroyed(const TMsgEntityDestroyed&) {
    if (_particles) {
        EngineParticles.kill(_particles->getHandle(), _fadeOut);
    }
}

void TCompParticles::onParticleSystemDestroyed(const TMsgParticleSystemDestroyed&) {
	_particles = nullptr;
	//TODO decidir si hay que destruir el componente
}

void TCompParticles::forceEmission(int quantity) {
	if (_particles) {
		_particles->forceEmission(quantity);
	}
}

void TCompParticles::setOffset(VEC3 offset) {
	this->offset = offset;
	if (_particles) {
		_particles->setOffset(offset);
	}
}

void TCompParticles::setRotationOffset(QUAT rotationOffset) {
	this->rotationOffset = rotationOffset;
	if (_particles) {
		_particles->setRotationOffset(rotationOffset);
	}
}
