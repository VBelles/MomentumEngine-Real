#include "mcv_platform.h"
#include "basic_controller_filter_callback.h"


bool BasicControllerFilterCallback::filter(const PxController& a, const PxController& b) {
	PxFilterData filterData = getFilterData(&a);
	PxFilterData filterData1 = getFilterData(&b);
	if ((filterData.word0 & filterData1.word1) && (filterData1.word0 & filterData.word1)) {
		return true;
	}
	return false;
}