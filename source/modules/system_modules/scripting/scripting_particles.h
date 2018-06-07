#pragma once

namespace SLB {
	class Manager;
}

class ScriptingParticles {
private:

	static void bindConstants(SLB::Manager* manager);

public:
	static void bind(SLB::Manager* manager);

	static int launchParticleAtPos(const std::string& name, float x, float y, float z);
	static int launchParticleAtEntity(const std::string& name, const std::string& entityName);
	static int launchParticleAtEntityOffset(const std::string& name, const std::string& entityName, float x, float y, float z);
	static int launchParticleAtBone(const std::string& name, const std::string& entityName, const std::string& bone);
	static int launchParticleAtBoneOffset(const std::string& name, const std::string& entityName, const std::string& bone, float x, float y, float z);
	static void killParticles(int ph, float fade_out);
};
