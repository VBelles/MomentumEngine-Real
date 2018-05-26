#include "mcv_platform.h"
#include "EStates.h"


namespace States {
	const char* toString(State state) {
		return strStates[state];
	}

	const char* toString(ConcurrentState state) {
		return strConcurrentStates[state];
	}
}