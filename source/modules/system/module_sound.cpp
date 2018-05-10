#include "mcv_platform.h"
#include "module_sound.h"


CModuleSound::CModuleSound(const std::string& name) : IModule(name) {
}

bool CModuleSound::start() {
	return true;
}

bool CModuleSound::stop() {
	return true;
}

void CModuleSound::update(float delta) {
}


