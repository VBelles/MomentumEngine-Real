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
	res = system->initialize(1024, FMOD_STUDIO_INIT_ALLOW_MISSING_PLUGINS, FMOD_INIT_3D_RIGHTHANDED, extraDriverData);
	//extraDriverData.
	system->setListenerAttributes(0, &listenerAttributes);
	system->getLowLevelSystem(&lowLevelSystem);
	assert(res == FMOD_OK);

	auto j = loadJson("data/sounds.json");
	for (const std::string& bankFile : j["banks"]) {
		Studio::Bank* bank = nullptr;
		res = system->loadBankFile(bankFile.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
		dbg("Loading bank: %s\n", bankFile.c_str());
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
	updateFollowingEvents();
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

void CModuleSound::updateFollowingEvents() {
	auto it = followingEvents.begin();
	while (it != followingEvents.end()) {
		auto& followingEvent = *it;
		if (!followingEvent.eventInstance->isValid() || !followingEvent.transformHandle.isValid()) { // Event has been released or does not follow a transform anymore
			it = followingEvents.erase(it);
		}
		else {
			TCompTransform* transform = followingEvent.transformHandle;
			FMOD_3D_ATTRIBUTES attributes = toFMODAttributes(*transform);
			followingEvent.eventInstance->set3DAttributes(&attributes);
			++it;
		}
	}
}

Studio::EventInstance* CModuleSound::emitFollowingEvent(const std::string& sound, CHandle transformHandle) {
	TCompTransform* transform = transformHandle;
	auto eventInstance = emitEvent(sound, transform);
	followingEvents.push_back(FollowingEvent{ eventInstance, transformHandle });
	return eventInstance;
}

Studio::EventInstance* CModuleSound::emitEvent(const std::string& sound, const CTransform* transform) {
	FMOD_3D_ATTRIBUTES attributes = toFMODAttributes(*transform);
	return emitEvent(sound, &attributes);
}

Studio::EventInstance* CModuleSound::emitEvent(const std::string& sound, const FMOD_3D_ATTRIBUTES* attributes) {
	Studio::EventInstance* eventInstance = getEventInstance(sound, attributes);
	eventInstance->start();
	eventInstance->release();
	return eventInstance;
}

FMOD::Studio::EventInstance * CModuleSound::getEventInstance(const std::string & sound, const CTransform * transform) {
	FMOD_3D_ATTRIBUTES attributes = toFMODAttributes(*transform);
	return getEventInstance(sound, &attributes);
}

FMOD::Studio::EventInstance * CModuleSound::getEventInstance(const std::string & sound, const FMOD_3D_ATTRIBUTES * attributes) {
	if (sound.empty()) return nullptr;
	Studio::EventDescription* descriptor = nullptr;
	Studio::EventInstance* eventInstance = nullptr;
	res = system->getEvent(sound.c_str(), &descriptor);
	if (!descriptor) {
		dbg("Event %s not found\n", sound.c_str());
		return nullptr;
	}
	res = descriptor->createInstance(&eventInstance);
	eventInstance->set3DAttributes(attributes);
	return eventInstance;
}

FMOD::Studio::EventDescription * CModuleSound::getEventDescription(const std::string & sound) {
	if (sound.empty()) return nullptr;
	Studio::EventDescription* descriptor = nullptr;
	res = system->getEvent(sound.c_str(), &descriptor);
	if (!descriptor) {
		dbg("Event %s not found\n", sound.c_str());
		return nullptr;
	}
	return descriptor;
}

FMOD::Studio::EventInstance * CModuleSound::emitEventFromDescriptor(FMOD::Studio::EventDescription * descriptor, const CTransform * transform) {
	FMOD_3D_ATTRIBUTES attributes = toFMODAttributes(*transform);
	return emitEventFromDescriptor(descriptor, &attributes);
}

FMOD::Studio::EventInstance* CModuleSound::emitEventFromDescriptor(FMOD::Studio::EventDescription * descriptor, FMOD_3D_ATTRIBUTES* attributes) {
	Studio::EventInstance* eventInstance = nullptr;
	res = descriptor->createInstance(&eventInstance);
	eventInstance->set3DAttributes(attributes);
	eventInstance->start();
	eventInstance->release();
	return eventInstance;
}

FMOD::Studio::System* CModuleSound::getSystem() {
	return system;
}

void CModuleSound::stopEvent(Studio::EventInstance* instance, bool fadeout) {
	if (instance && instance->isValid()) {
		instance->stop(fadeout ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE);
	}
}

void CModuleSound::setVolume(float volume) {
	masterVolume = volume;
	FMOD::ChannelGroup* masterChannelGroup = nullptr;
	lowLevelSystem->getMasterChannelGroup(&masterChannelGroup);
	masterChannelGroup->setVolume(masterVolume);
	/*for (auto bank : getBanks()) {
		for (auto description : getEventDescriptions(bank)) {
			for (auto eventInstance : getEventInstances(description)) {
				eventInstance->setVolume(10);
			}
		}
	}*/
	
}

std::vector<Studio::Bank*> CModuleSound::getBanks() {
	int bankCount = 0;
	system->getBankCount(&bankCount);
	std::vector<Studio::Bank*> banks;
	banks.resize(bankCount);
	system->getBankList(&banks[0], bankCount, &bankCount);
	return banks;
}


std::vector<Studio::EventDescription*> CModuleSound::getEventDescriptions(Studio::Bank* bank) {
	int descriptionsCount = 0;
	bank->getEventCount(&descriptionsCount);
	std::vector<Studio::EventDescription*> descriptions;
	descriptions.resize(descriptionsCount);
	bank->getEventList(&descriptions[0], descriptionsCount, &descriptionsCount);
	return descriptions;
}

std::vector<FMOD::Studio::EventInstance*> CModuleSound::getEventInstances(Studio::EventDescription * eventDescription) {
	int eventsCount = 0;
	eventDescription->getInstanceCount(&eventsCount);
	std::vector<Studio::EventInstance*> events;
	events.resize(eventsCount);
	eventDescription->getInstanceList(&events[0], eventsCount, &eventsCount);
	return events;
}

void CModuleSound::render() {
	if (CApp::get().isDebug()) {
		if (ImGui::TreeNode("Sound")) {
			if (ImGui::DragFloat("Master volume", &masterVolume, 0.01f, 0.f, 1.f)) {
				setVolume(masterVolume);
			}
			int bankCount = 0;
			system->getBankCount(&bankCount);
			std::vector<Studio::Bank*> banks;
			banks.resize(bankCount);
			system->getBankList(&banks[0], bankCount, &bankCount);
			ImGui::Text("Banks: %d", bankCount);

			static ImGuiTextFilter Filter;
			Filter.Draw("Filter");

			ImGui::Text("Following events: %d", followingEvents.size());
			for (auto& followingEvent : followingEvents) {
				Studio::EventDescription* eventDescription = nullptr;
				followingEvent.eventInstance->getDescription(&eventDescription);
				char path[256];
				eventDescription->getPath(path, 256, nullptr);
				if (!Filter.IsActive() || Filter.PassFilter(path)) {
					int instancesCount = 0;
					eventDescription->getInstanceCount(&instancesCount);
					TCompTransform* transform = followingEvent.transformHandle;
					CEntity* owner = followingEvent.transformHandle.getOwner();
					ImGui::Text("(%d) %s", instancesCount, path);
					ImGui::Text("Target: %s", owner->getName());
					if (ImGui::TreeNode("Transform")) {
						transform->debugInMenu();
						ImGui::TreePop();
					}
				}
			}

			for (auto bank : banks) {
				char bankPath[256];
				bank->getPath(bankPath, 256, nullptr);
				if (ImGui::TreeNode("Bank: %s", bankPath)) {
					int eventCount = 0;
					bank->getEventCount(&eventCount);
					//if (eventCount == 0) continue;
					std::vector<Studio::EventDescription*> events;
					events.resize(eventCount);
					bank->getEventList(&events[0], eventCount, &eventCount);
					//if (eventCount == 0) continue;
					ImGui::Text("Events: %d", eventCount);
					for (auto event : events) {
						char path[256];
						event->getPath(path, 256, nullptr);
						if (!Filter.IsActive() || Filter.PassFilter(path)) {
							int instancesCount = 0;
							event->getInstanceCount(&instancesCount);
							ImGui::Text("(%d) %s", instancesCount, path);
						}
					}
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
	}
}



