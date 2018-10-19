#include "mcv_platform.h"
#include "comp_fire_puzzle.h"
#include "entity/common_msgs.h"
#include "components/comp_collectable.h"


DECL_OBJ_MANAGER("fire_puzzle", TCompFirePuzzle);

void TCompFirePuzzle::debugInMenu() {
}

void TCompFirePuzzle::registerMsgs() {
	DECL_MSG(TCompFirePuzzle, TMsgEntitiesGroupCreated, onEntitGroupCreated);
	DECL_MSG(TCompFirePuzzle, TMsgFireTurnOff, onFireTurnOff);

}

void TCompFirePuzzle::load(const json& j, TEntityParseContext& ctx) {
	goal = j.value("goal", goal);
}

void TCompFirePuzzle::onEntitGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	turnedOffFires = 0;
	TCompRender* render = get<TCompRender>();
	TCompCollectable* collectable = get<TCompCollectable>();
	render->disable();
	collectable->setActive(false);
	fires.clear();
}

void TCompFirePuzzle::onFireTurnOff(const TMsgFireTurnOff& msg) {
	auto pair = fires.insert(msg.fireEntity);
	if (pair.second) {
		dbg("Fire turned off\n");
		turnedOffFires++;

		if (turnedOffFires >= goal) {
			dbg("Goal!!\n");
			TCompRender* render = get<TCompRender>();
			TCompCollectable* collectable = get<TCompCollectable>();
			render->enable();
			collectable->setActive(true);			
		}
	}
}



