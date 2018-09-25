#include "mcv_platform.h"
#include "module_sound.h"
#include "components/comp_camera.h"

#pragma comment(lib, "fmod64_vc.lib")
#pragma comment(lib, "fmodstudio64_vc.lib")

using namespace FMOD;

CModuleSound::CModuleSound(const std::string& name) : IModule(name) {
}

bool CModuleSound::start() {
	res = Studio::System::create(&system);
	assert(res == FMOD_OK);
	res = system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_3D_RIGHTHANDED, extraDriverData);
	system->setListenerAttributes(0, &listenerAttributes);
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
	updateListenerAttributes();
	system->update();
}

void CModuleSound::updateListenerAttributes() {
	if (!cameraHandle.isValid()) {
		CHandle cameraEntityHandle = getEntityByName(GAME_CAMERA);
		if (cameraEntityHandle.isValid()) {
			CEntity* cameraEntity = cameraEntityHandle;
			cameraHandle = cameraEntity->get<TCompCamera>();
		}
	}
	if (cameraHandle.isValid()) {
		TCompCamera* camera = cameraHandle;
		listenerAttributes.position = toFMODVector(camera->getCamera()->getPosition());
		listenerAttributes.forward = toFMODVector(camera->getCamera()->getFront());
		listenerAttributes.up = toFMODVector(camera->getCamera()->getUp());
		listenerAttributes.velocity = {};
	}
	else {
		listenerAttributes = {};
	}
	auto res = system->setListenerAttributes(0, &listenerAttributes);


}

Studio::EventInstance* CModuleSound::emitEvent(const char* sound, const CTransform& transform) {
	FMOD_3D_ATTRIBUTES attributes = toFMODAttributes(transform);
	return emitEvent(sound, &attributes);
}

Studio::EventInstance* CModuleSound::emitEvent(const char* sound, const FMOD_3D_ATTRIBUTES* attributes) {
	Studio::EventDescription* descriptor = nullptr;
	Studio::EventInstance* eventInstance = nullptr;
	res = system->getEvent(sound, &descriptor);
	if (!descriptor) {
		dbg("Event %s not found\n", sound);
		return nullptr;
	}
	res = descriptor->createInstance(&eventInstance);
	eventInstance->set3DAttributes(attributes);
	eventInstance->start();
	eventInstance->release();
	return eventInstance;
}

FMOD::Studio::System* CModuleSound::getSystem() {
	return system;
}



