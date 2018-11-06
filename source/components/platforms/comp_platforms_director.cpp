#include "mcv_platform.h"
#include "comp_platforms_director.h"
#include "comp_platform_simple.h"
#include "entity/entity_parser.h"
#include "components/controllers/comp_rigid_anim.h"

DECL_OBJ_MANAGER("platforms_director", TCompPlatformsDirector);

void TCompPlatformsDirector::debugInMenu() {
	if (ImGui::Checkbox("Enabled", &enabled)) {
		int previousSlot = (currentSlot + platformHandles.size() - 1) % platformHandles.size();
		for (TCompPlatformSimple* platform : platformHandles[previousSlot]) {
			platform->setEnabled(enabled);
		}
		if (enabled) {
			timer.setElapsed(elapsedAtPause);
		}
		else {
			elapsedAtPause = timer.elapsed();
		}
	}
}

void TCompPlatformsDirector::setEnabled(bool enabled) {
	if (this->enabled != enabled) {
		this->enabled = enabled;
		int previousSlot = (currentSlot + platformHandles.size() - 1) % platformHandles.size();
		for (TCompPlatformSimple* platform : platformHandles[previousSlot]) {
			platform->setEnabled(enabled);
		}
		if (enabled) {
			timer.setElapsed(elapsedAtPause);
		}
		else {
			elapsedAtPause = timer.elapsed();
		}
	}
}

void TCompPlatformsDirector::load(const json& j, TEntityParseContext& ctx) {
	platformEntitiesNames.clear();
	waitTimes.clear();
	for (auto& jSlot : j["slots"]) {
		std::vector<std::string> v;
		for (std::string platformName : jSlot["platforms"]) {
			v.push_back(platformName);
		}
		platformEntitiesNames.push_back(v);
		float waitTime = jSlot.value("wait_time", timeBetweenSlots);
		waitTimes.push_back(waitTime);
	}
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
	if (enabled) {
		if (timer.elapsed() >= nextSlotTime) {
			nextSlotTime = timer.elapsed() + waitTimes[currentSlot];

			for (TCompPlatformSimple* platform : platformHandles[currentSlot]) {
				platform->turnAround();
			}

			currentSlot = (currentSlot + 1) % platformHandles.size();
		}
	}
}
