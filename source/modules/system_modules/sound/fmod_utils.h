#pragma once

namespace FMOD_UTILS {
	std::vector<FMOD::Studio::Bank*> getBanks(FMOD::Studio::System* system);
	std::vector<FMOD::Studio::EventDescription*> getEventDescriptions(FMOD::Studio::Bank* bank);
	std::vector<FMOD::Studio::EventInstance*> getEventInstances(FMOD::Studio::EventDescription* eventDescription);
	std::vector<FMOD::Studio::Bus*> getBuses(FMOD::Studio::Bank* bank);

	template <typename T>
	std::string getPath(T* t) {
		char path[256];
		t->getPath(path, 256, nullptr);
		return path;
	}

}




