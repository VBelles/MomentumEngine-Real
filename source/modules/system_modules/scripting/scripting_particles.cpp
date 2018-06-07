#include "mcv_platform.h"
#include "scripting_particles.h"
#include <SLB/SLB.hpp>

void ScriptingParticles::bind(SLB::Manager* manager) {
	bindConstants(manager);
	manager->set("launchParticleAtPos", SLB::FuncCall::create(ScriptingParticles::launchParticleAtPos));
	manager->set("launchParticleAtEntity", SLB::FuncCall::create(ScriptingParticles::launchParticleAtEntity));
	manager->set("launchParticleAtEntityOffset", SLB::FuncCall::create(ScriptingParticles::launchParticleAtEntityOffset));
	manager->set("launchParticleAtBone", SLB::FuncCall::create(ScriptingParticles::launchParticleAtBone));
	manager->set("launchParticleAtBoneOffset", SLB::FuncCall::create(ScriptingParticles::launchParticleAtBoneOffset));
	manager->set("killParticle", SLB::FuncCall::create(ScriptingParticles::killParticles));
}

void ScriptingParticles::bindConstants(SLB::Manager* manager) {
	manager->set("PARTICLE_ANIMATED_FIRE", SLB::Value::copy((std::string)"data/particles/animated_fire.particles"));
	manager->set("PARTICLE_SNOW", SLB::Value::copy((std::string)"data/particles/snow.particles"));
	manager->set("PARTICLE_SPARKS", SLB::Value::copy((std::string)"data/particles/sparks.particles"));
	manager->set("PARTICLE_WILDFIRE", SLB::Value::copy((std::string)"data/particles/wildfire.particles"));
}

int ScriptingParticles::launchParticleAtPos(const std::string& name, float x, float y, float z) {
	return EngineParticles.launchSystem(name, VEC3(x, y, z));
}

int ScriptingParticles::launchParticleAtEntity(const std::string& name, const std::string& entityName) {
	return launchParticleAtBoneOffset(name, entityName, "", 0, 0, 0);
}

int ScriptingParticles::launchParticleAtEntityOffset(const std::string& name, const std::string& entityName, float x, float y, float z) {
	return launchParticleAtBoneOffset(name, entityName, "", x, y, z);
}

int ScriptingParticles::launchParticleAtBone(const std::string& name, const std::string& entityName, const std::string& bone) {
	return launchParticleAtBoneOffset(name, entityName, bone, 0, 0, 0);
}

int ScriptingParticles::launchParticleAtBoneOffset(const std::string& name, const std::string& entityName, const std::string& bone, float x, float y, float z) {
	CHandle entityHandle = getEntityByName(entityName);
	if (entityHandle.isValid()) {
		return EngineParticles.launchSystem(name, entityHandle, bone, VEC3(x, y, z));
	}
	else {
		return -1;
	}
}

void ScriptingParticles::killParticles(int ph, float fade_out) {
	EngineParticles.kill(ph, fade_out);
}