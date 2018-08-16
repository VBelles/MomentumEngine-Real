#include "mcv_platform.h"
#include "comp_platforms_director.h"
#include "comp_platform_simple.h"
#include "components/comp_transform.h"
#include "entity/entity_parser.h"
#include "components/controllers/comp_rigid_anim.h"

DECL_OBJ_MANAGER("platforms_director", TCompPlatformsDirector);

void TCompPlatformsDirector::load(const json& j, TEntityParseContext& ctx) {

	for (auto& jPlatforms : j["platforms"]) {
		std::vector<std::string> v;
		for (std::string platformName : jPlatforms) {
			v.push_back(platformName);
		}
		platformEntitiesNames.push_back(v);
	}
	timeBetweenSlots = j.value("wait_time", timeBetweenSlots);

}

void TCompPlatformsDirector::debugInMenu() {
}

void TCompPlatformsDirector::registerMsgs() {
	DECL_MSG(TCompPlatformsDirector, TMsgEntitiesGroupCreated, onGroupCreated);
}


void TCompPlatformsDirector::onGroupCreated(const TMsgEntitiesGroupCreated & msg) {
	for (auto& vector : platformEntitiesNames) {
		std::vector<CHandle> v;
		for (std::string name : vector) {
			CEntity* entity = getEntityByName(name);
			assert(entity);
			CHandle handle = entity->get<TCompPlatformSimple>();
			if (handle.isValid()) {
				((TCompPlatformSimple*)handle)->hasDirector = true;
				v.push_back(handle);
			}
		}
		platformHandles.push_back(v);
	}
}

void TCompPlatformsDirector::update(float delta) {
	if (timer.elapsed() >= nextSlotTime) {
		nextSlotTime = timer.elapsed() + timeBetweenSlots;

		for (TCompPlatformSimple* platform : platformHandles[currentSlot]) {
			platform->turnAround();
		}

		currentSlot = (currentSlot + 1) % platformHandles.size();
	}
}