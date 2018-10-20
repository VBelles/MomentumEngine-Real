#include "mcv_platform.h"
#include "comp_fire_puzzle.h"
#include "entity/common_msgs.h"
#include "components/comp_collectable.h"
#include "entity/entity_parser.h"
#include "components/comp_name.h"

DECL_OBJ_MANAGER("fire_puzzle", TCompFirePuzzle);

void TCompFirePuzzle::debugInMenu() {
}

void TCompFirePuzzle::registerMsgs() {
	DECL_MSG(TCompFirePuzzle, TMsgEntitiesGroupCreated, onEntitGroupCreated);
	DECL_MSG(TCompFirePuzzle, TMsgFireTurnOff, onFireTurnOff);

}

void TCompFirePuzzle::load(const json& j, TEntityParseContext& ctx) {
	goal = j.value("goal", goal);
	chrysalisName = j.value("chrysalis", chrysalisName);
}

void TCompFirePuzzle::onEntitGroupCreated(const TMsgEntitiesGroupCreated& msg) {
	turnedOffFires = 0;
	fires.clear();
	auto unique = EngineUniques.getUniqueChrysalis(chrysalisName);
	assert(unique);
}

void TCompFirePuzzle::onFireTurnOff(const TMsgFireTurnOff& msg) {
	auto pair = fires.insert(msg.fireEntity);
	if (pair.second) {
		//dbg("Fire turned off\n");
		turnedOffFires++;

		if (turnedOffFires >= goal) {
			dbg("Puzzle solved!!\n");
			if (!done) {
				EngineSound.emitEvent(SOUND_CHRYSALIS_PUZZLE);

				CEntity* owner = CHandle(this).getOwner();
				auto unique = EngineUniques.getUniqueChrysalis(chrysalisName);
				if (!unique->done) {
					// Spawn chrysalis
					TEntityParseContext ctx;
					ctx.root_transform.setPosition(unique->position);
					parseScene(PREFAB_CHRYSALIS, ctx);
					CEntity* chrysalis = ctx.entities_loaded[0];
					TCompName* name = chrysalis->get<TCompName>();
					name->setName(chrysalisName.c_str());
				}
				else {
					// Spawn coin
					TEntityParseContext ctx;
					ctx.root_transform.setPosition(unique->position);
					parseScene(PREFAB_COIN, ctx);
				}



			}

		}
	}
}



