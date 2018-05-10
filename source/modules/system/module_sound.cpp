#include "mcv_platform.h"
#include "module_sound.h"

#pragma comment(lib, "fmod64_vc.lib")
#pragma comment(lib, "fmodstudio64_vc.lib")

using namespace FMOD;

CModuleSound::CModuleSound(const std::string& name) : IModule(name) {
}

bool CModuleSound::start() {
	FMOD_RESULT res;
	res = Studio::System::create(&system);
	res = system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData);
	return true;
}

bool CModuleSound::stop() {
	FMOD_RESULT res;
	res = system->release();
	return true;
}

void CModuleSound::update(float delta) {
	system->update();
}


