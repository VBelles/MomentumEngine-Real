#include "mcv_platform.h"
#include "module_sound.h"
#include "components/comp_camera.h"
#include "components/comp_transform.h"

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

Studio::EventInstance* CModuleSound::emitFollowingEvent(const char* sound, CHandle transformHandle) {
	TCompTransform* transform = transformHandle;
	auto eventInstance = emitEvent(sound, *transform);
	followingEvents.push_back(FollowingEvent{ eventInstance, transformHandle });
	return eventInstance;
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

void CModuleSound::stopEvent(Studio::EventInstance* instance, bool fadeout) {
	if (instance && instance->isValid()) {
		instance->stop(fadeout ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE);
	}
}

void CModuleSound::render() {
	if (CApp::get().isDebug()) {
		if (ImGui::TreeNode("Sound")) {
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
				int eventCount = 0;
				bank->getEventCount(&eventCount);
				if (eventCount == 0) continue;
				std::vector<Studio::EventDescription*> events;
				events.resize(eventCount);
				bank->getEventList(&events[0], eventCount, &eventCount);
				if (eventCount == 0) continue;
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
	}
}



