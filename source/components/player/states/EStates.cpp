#include "mcv_platform.h"
#include "EStates.h"


namespace States {
	const char* toString(State state) {
		return strStates[state];
	}

	const char* toString(ConcurrentState state) {
		return strConcurrentStates[state];
	}

	State getState(std::string state) {
		auto& it = statesMap.find(state);
		if (it != statesMap.end()) {
			return it->second;
		}
		else {
			return UndefinedState;
		}
	}

	ConcurrentState getConcurrentState(std::string state) {
		auto& it = concurrentStatesMap.find(state);
		if (it != concurrentStatesMap.end()) {
			return it->second;
		}
		else {
			return UndefinedConcurrentState;
		}
	}
}
