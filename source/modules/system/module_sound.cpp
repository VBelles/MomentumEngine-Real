#include "mcv_platform.h"
#include "module_sound.h"

#pragma comment(lib, "fmod64_vc.lib")
#pragma comment(lib, "fmodstudio64_vc.lib")

using namespace FMOD;

CModuleSound::CModuleSound(const std::string& name) : IModule(name) {
}

bool CModuleSound::start() {
	res = Studio::System::create(&system);
	res = system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData);

	const int maxLen = 255;
	char buffer[maxLen + 1];

	json j = loadJson("data/sounds.json");
	for (std::string bankFile : j["banks"]) {
		Studio::Bank* bank = nullptr;
		res = system->loadBankFile(bankFile.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
		assert(res == FMOD_OK);
		int eventCount = 0;
		bank->getEventCount(&eventCount);
		if (!eventCount) continue;
		std::vector<Studio::EventDescription*> eventsDescription(eventCount);
		bank->getEventList(&eventsDescription[0], eventCount, &eventCount);
		for (auto event : eventsDescription) {
			std::string eventPath;
			event->getPath(&eventPath[0], maxLen, 0);

			dbg("%s\n", eventPath.c_str());

		}
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

void CModuleSound::instanceEvent(const char* event) {
	if (eventInstances.find(event) == eventInstances.end()) {
		Studio::EventDescription* descriptor = nullptr;
		res = system->getEvent(event, &descriptor);
		Studio::EventInstance* eventInstance = nullptr;
		res = descriptor->createInstance(&eventInstance);
		eventInstances[event] = eventInstance;
	}
	else {
		dbg("Event '%s' already instanced\n", event);
	}
}

void CModuleSound::releaseEvent(const char* event) {
	try {
		eventInstances.at(event)->release();
		eventInstances.erase(event);
	}
	catch (const std::out_of_range& e) {
		dbg("Releasing an uninstanced event\n");
	}
}

void CModuleSound::startEvent(const char* event) {
	try {
		eventInstances.at(event)->start();
	}
	catch (const std::out_of_range& e) {
		dbg("Starting an uninstanced event\n");
	}
}

void CModuleSound::stopEvent(const char* event, FMOD_STUDIO_STOP_MODE mode) {
	try {
		eventInstances.at(event)->stop(mode);
	}
	catch (const std::out_of_range& e) {
		dbg("Stoping an uninstanced event\n");
	}
}

void CModuleSound::emitEvent(const char* event) {
	Studio::EventDescription* descriptor = nullptr;
	res = system->getEvent(event, &descriptor);
	Studio::EventInstance* eventInstance = nullptr;
	res = descriptor->createInstance(&eventInstance);
	eventInstance->start();
	eventInstance->release();
}



