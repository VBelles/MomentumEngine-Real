#include "mcv_platform.h"
#include "fmod_utils.h"

using namespace FMOD;
namespace FMOD_UTILS {

	std::vector<Studio::Bank*> getBanks(Studio::System* system) {
		int bankCount = 0;
		system->getBankCount(&bankCount);
		std::vector<Studio::Bank*> banks;
		banks.resize(bankCount);
		system->getBankList(&banks[0], bankCount, &bankCount);
		return banks;
	}

	std::vector<Studio::EventDescription*> getEventDescriptions(Studio::Bank* bank) {
		int descriptionsCount = 0;
		bank->getEventCount(&descriptionsCount);
		std::vector<Studio::EventDescription*> descriptions;
		descriptions.resize(descriptionsCount);
		bank->getEventList(&descriptions[0], descriptionsCount, &descriptionsCount);
		return descriptions;
	}

	std::vector<Studio::EventInstance*> getEventInstances(Studio::EventDescription * eventDescription) {
		int eventsCount = 0;
		eventDescription->getInstanceCount(&eventsCount);
		std::vector<Studio::EventInstance*> events;
		events.resize(eventsCount);
		eventDescription->getInstanceList(&events[0], eventsCount, &eventsCount);
		return events;
	}

	std::vector<Studio::Bus*> getBuses(Studio::Bank* bank) {
		int busCount = 0;
		bank->getBusCount(&busCount);
		std::vector<Studio::Bus*> buses;
		buses.resize(busCount);
		bank->getBusList(&buses[0], busCount, &busCount);
		return buses;
	}


}

