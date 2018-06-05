#include "mcv_platform.h"
#include "physx_utils.h"
#include "components/comp_collider.h"

namespace PhysxUtils {

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

	CHandle getCollider(const physx::PxActor* actor) {
		CHandle handle;
		handle.fromVoidPtr(actor->userData);
		assert(handle.isValid());
		return handle;
	}

	CHandle getEntity(const physx::PxActor* actor) {
		CHandle handle;
		handle.fromVoidPtr(actor->userData);
		assert(handle.isValid());
		return handle.getOwner();
	}

}