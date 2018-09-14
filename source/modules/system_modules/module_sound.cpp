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
	res = system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_3D_RIGHTHANDED, extraDriverData);
	system->getLowLevelSystem(&lowLevelSystem);
	assert(res == FMOD_OK);

	auto j = loadJson("data/sounds.json");
	for (const std::string& bankFile : j["banks"]) {
		Studio::Bank* bank = nullptr;
		res = system->loadBankFile(bankFile.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);

		assert(res == FMOD_OK);
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
	lowLevelSystem->set3DListenerAttributes(0, &listenerAttributes.position, &listenerAttributes.velocity, &listenerAttributes.forward, &listenerAttributes.up);
	system->update();
}

void CModuleSound::updateListenerAttributes(VEC3& position, VEC3& velocity, VEC3& forward, VEC3& up) {
	listenerAttributes.position = { position.x, position.y, position.z };
	listenerAttributes.velocity = { velocity.x, velocity.y, velocity.z };
	listenerAttributes.forward = { forward.x, forward.y, forward.z };
	listenerAttributes.up = { up.x, up.y, up.z };
}

Studio::EventInstance* CModuleSound::instanceEvent(const char* sound) {
	auto it = eventInstances.find(sound);
	if (it == eventInstances.end()) {
		Studio::EventDescription* descriptor = nullptr;
		res = system->getEvent(sound, &descriptor);
		Studio::EventInstance* eventInstance = nullptr;
		res = descriptor->createInstance(&eventInstance);
		eventInstances[sound] = eventInstance;
		return eventInstance;
	}
	else {
		return it->second;
	}
}

void CModuleSound::releaseEvent(const char* sound) {
	auto it = eventInstances.find(sound);
	if (it != eventInstances.end()) {
		it->second->release();
		eventInstances.erase(sound);
	}
}

void CModuleSound::startEvent(const char* sound, const FMOD_3D_ATTRIBUTES* attributes) {
	auto it = eventInstances.find(sound);
	Studio::EventInstance* eventInstance = it->second;
	if (it == eventInstances.end()) {
		eventInstance = instanceEvent(sound);
	}
	if (eventInstance) {
		eventInstance->set3DAttributes(attributes);
		eventInstance->start();
	}
	else {
		dbg("Event instance not found\n");
	}
}

void CModuleSound::stopEvent(const char* sound, FMOD_STUDIO_STOP_MODE mode) {
	auto it = eventInstances.find(sound);
	if (it != eventInstances.end()) {
		it->second->stop(mode);
	}
}

void CModuleSound::emitEvent(const char* sound, VEC3 position, VEC3 velocity, VEC3 forward, VEC3 up) {
	FMOD_3D_ATTRIBUTES attributes;
	attributes.position = { position.x, position.y, position.z };
	attributes.velocity = { velocity.x, velocity.y, velocity.z };
	attributes.forward = { forward.x, forward.y, forward.z };
	attributes.up = { up.x, up.y, up.z };
	emitEvent(sound, &attributes);
}

void CModuleSound::emitEvent(const char* sound, const FMOD_3D_ATTRIBUTES* attributes) {
	Studio::EventDescription* descriptor = nullptr;
	res = system->getEvent(sound, &descriptor);
	Studio::EventInstance* eventInstance = nullptr;
	res = descriptor->createInstance(&eventInstance);
	if (eventInstance) {
		eventInstance->set3DAttributes(attributes);
		eventInstance->start();
		eventInstance->release();
	}
	else {
		dbg("Event instance not found\n");
	}
}
