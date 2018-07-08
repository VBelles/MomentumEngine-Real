#include "mcv_platform.h"
#include "module_sound.h"

#pragma comment(lib, "fmod64_vc.lib")
#pragma comment(lib, "fmodstudio64_vc.lib")

using namespace FMOD;

CModuleSound::CModuleSound(const std::string& name) : IModule(name) {
}

bool CModuleSound::start() {
	res = Studio::System::create(&system);
	assert(res == FMOD_OK);
	res = system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData);
	assert(res == FMOD_OK);

	json j = loadJson("data/sounds.json");
	for (std::string bankFile : j["banks"]) {
		Studio::Bank* bank = nullptr;
		res = system->loadBankFile(bankFile.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
		
		assert(res == FMOD_OK);
		banks[bankFile] = bank;
	}
	//instanceEvent(TEST_EVENT);
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

void CModuleSound::instanceEvent(std::string event) {
	if (eventInstances.find(event) == eventInstances.end()) {
		Studio::EventDescription* descriptor = nullptr;
		res = system->getEvent(event.c_str(), &descriptor);
		Studio::EventInstance* eventInstance = nullptr;
		res = descriptor->createInstance(&eventInstance);
		eventInstances[event] = eventInstance;
	}
	else {
		dbg("Event '%s' already instanced\n", event);
	}
}

void CModuleSound::releaseEvent(std::string event) {
	try {
		eventInstances.at(event)->release();
		eventInstances.erase(event);
	}
	catch (const std::out_of_range& e) {
        UNREFERENCED_PARAMETER(e);
		dbg("Releasing an uninstanced event\n");
	}
}

void CModuleSound::startEvent(std::string event) {
	try {
		eventInstances.at(event.c_str())->start();
	}
	catch (const std::out_of_range& e) {
        UNREFERENCED_PARAMETER(e);
		dbg("Starting an uninstanced event\n");
	}
}

void CModuleSound::stopEvent(std::string event, FMOD_STUDIO_STOP_MODE mode) {
	try {
		eventInstances.at(event.c_str())->stop(mode);
	}
	catch (const std::out_of_range& e) {
        UNREFERENCED_PARAMETER(e);
		dbg("Stoping an uninstanced event\n");
	}
}

void CModuleSound::emitEvent(std::string event) {
	Studio::EventDescription* descriptor = nullptr;
	res = system->getEvent(event.c_str(), &descriptor);
	Studio::EventInstance* eventInstance = nullptr;
	res = descriptor->createInstance(&eventInstance);
	//dbg("RES: %d\n", res);
	eventInstance->start();
	eventInstance->release();
}
