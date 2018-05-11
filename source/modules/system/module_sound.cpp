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

	json j = loadJson("data/sounds.json");
	for (std::string bankFile : j["banks"]) {
		Studio::Bank* bank = nullptr;
		res = system->loadBankFile(bankFile.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
		assert(res == 0);
		banks[bankFile] = bank;
	}

	return true;
}

bool CModuleSound::stop() {
	for (auto& p : banks) {
		p.second->unload();
	}
	FMOD_RESULT res;
	res = system->release();
	banks.clear();
	return true;
}

void CModuleSound::update(float delta) {
	system->update();
}


