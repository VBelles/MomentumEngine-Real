#include "mcv_platform.h"
#include "module_sound.h"
#include "fmod_utils.h"
#include "music_player.h"

#pragma comment(lib, "fmod64_vc.lib")
#pragma comment(lib, "fmodstudio64_vc.lib")

using namespace FMOD;
using namespace FMOD_UTILS;

CModuleSound::CModuleSound(const std::string& name) : IModule(name) {
}

bool CModuleSound::start() {
	res = Studio::System::create(&system);
	assert(res == FMOD_OK);
	res = system->initialize(128, FMOD_STUDIO_INIT_ALLOW_MISSING_PLUGINS, FMOD_INIT_3D_RIGHTHANDED, extraDriverData);
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

	for (std::string event : j["unpausable_events"]) {
		std::transform(event.begin(), event.end(), event.begin(), ::tolower);
		unpausableEvents.insert(event);
	}

	soundsVCA = j.value("sounds_VCA", soundsVCA);
	musicVCA = j.value("music_VCA", musicVCA);

	setMasterVolume(Engine.globalConfig.masterVolume);
	setMusicVolume(Engine.globalConfig.musicVolume);
	setSoundVolume(Engine.globalConfig.soundVolume);

	musicPlayer = new CMusicPlayer(loadJson("data/sound/_test_music.json"));

	return true;
}

bool CModuleSound::stop() {
	musicPlayer->stop();
	safeDelete(musicPlayer);
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
	musicPlayer->update(delta);
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

void CModuleSound::setMasterVolume(float volume) {
	masterVolume = volume;
	FMOD::ChannelGroup* masterChannelGroup = nullptr;
	lowLevelSystem->getMasterChannelGroup(&masterChannelGroup);
	masterChannelGroup->setVolume(masterVolume);
}

void CModuleSound::setSoundVolume(float volume) {
	soundVolume = volume;
	Studio::VCA* vca = nullptr;
	system->getVCA(soundsVCA.c_str(), &vca);
	vca->setVolume(volume);
}

void CModuleSound::setMusicVolume(float volume) {
	musicVolume = volume;
	Studio::VCA* vca = nullptr;
	system->getVCA(musicVCA.c_str(), &vca);
	vca->setVolume(volume);
}

float CModuleSound::getMasterVolume() {
	return masterVolume;
}

float CModuleSound::getSoundVolume() {
	return soundVolume;
}

float CModuleSound::getMusicVolume() {
	return musicVolume;
}

void CModuleSound::setPaused(bool paused) {
	this->paused = paused;
	forEachEventInstance([this](FMOD::Studio::Bank* bank, FMOD::Studio::EventDescription* description, FMOD::Studio::EventInstance* event) {
		std::string path = getPath(description);
		std::transform(path.begin(), path.end(), path.begin(), ::tolower);
		if (unpausableEvents.find(path) == unpausableEvents.end()) {
			event->setPaused(this->paused);
		}
	});
}

void CModuleSound::forEachEventInstance(std::function<void(FMOD::Studio::Bank*, FMOD::Studio::EventDescription*, FMOD::Studio::EventInstance*)> callback) {
	for (auto bank : getBanks(getSystem())) {
		for (auto description : getEventDescriptions(bank)) {
			for (auto event : getEventInstances(description)) {
				callback(bank, description, event);
			}
		}
	}
}

void CModuleSound::forEachEventInstance(FMOD::Studio::Bank* bank, std::function<void(FMOD::Studio::EventDescription*, FMOD::Studio::EventInstance*)> callback) {
	for (auto description : getEventDescriptions(bank)) {
		for (auto event : getEventInstances(description)) {
			callback(description, event);
		}
	}

}

void CModuleSound::render() {
	if (CApp::get().isDebug()) {
		if (ImGui::TreeNode("Sound")) {
			if (ImGui::TreeNode("FMOD Cpu usage")) {
				FMOD_STUDIO_CPU_USAGE cpuUsage;
				system->getCPUUsage(&cpuUsage);
				ImGui::Text("DSP %f", cpuUsage.dspusage);
				ImGui::Text("Stream %f", cpuUsage.streamusage);
				ImGui::Text("Geometry %f", cpuUsage.geometryusage);
				ImGui::Text("Update %f", cpuUsage.updateusage);
				ImGui::Text("Studio %f", cpuUsage.studiousage);
				ImGui::TreePop();
			}
			if (ImGui::DragFloat("Master volume", &masterVolume, 0.01f, 0.f, 1.f)) {
				setMasterVolume(masterVolume);
			}
			if (ImGui::DragFloat("Sound volume", &soundVolume, 0.01f, 0.f, 1.f)) {
				setSoundVolume(soundVolume);
			}
			if (ImGui::DragFloat("Music volume", &musicVolume, 0.01f, 0.f, 1.f)) {
				setMusicVolume(musicVolume);
			}

			auto banks = getBanks(getSystem());
			ImGui::Text("Banks: %d", banks.size());

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

			int i = 0;
			for (auto bank : banks) {

				int eventCount = 0;
				bank->getEventCount(&eventCount);
				ImGui::PushID(i);
				if (ImGui::TreeNode("Bank: %s (%d)", getPath(bank).c_str(), eventCount)) {

					// Debug buses
					auto buses = getBuses(bank);
					if (ImGui::TreeNode("Buses")) {
						ImGui::Text("Count: %d", buses.size());
						for (auto bus : buses) {
							ImGui::Text("Bus: %s", getPath(bus).c_str());
						}
						ImGui::TreePop();
					}


					auto descriptions = getEventDescriptions(bank);
					if (ImGui::TreeNode("Events")) {
						ImGui::Text("Count: %d", descriptions.size());
						for (auto description : descriptions) {
							std::string path = getPath(description);
							if (!Filter.IsActive() || Filter.PassFilter(path.c_str())) {
								int instancesCount = 0;
								description->getInstanceCount(&instancesCount);
								ImGui::Text("(%d) %s", instancesCount, path.c_str());
							}
						}
						ImGui::TreePop();
					}
					ImGui::TreePop();
				}
				ImGui::PopID();
				i++;
			}


			ImGui::TreePop();
		}
	}
}

CMusicPlayer* CModuleSound::getMusicPlayer() {
	return musicPlayer;
}

