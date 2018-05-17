#include "mcv_platform.h"
#include "geometry_utils.h"


physx::PxFilterData getFilterData(const physx::PxController* controller) {
	physx::PxShape* tempShape;
	controller->getActor()->getShapes(&tempShape, 1);
	if (tempShape) {
		return tempShape->getSimulationFilterData();
	}
	else {
		return physx::PxFilterData();
	}
}