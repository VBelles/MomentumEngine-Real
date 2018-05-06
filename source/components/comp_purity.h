#pragma once

#include "comp_base.h"

class TCompRender;
class TCompCollider;
struct TMsgEntitiesGroupCreated;
struct TMsgPurityChange;

// If the player is in lvl power 1, enables collider. Else, disables it.
class TCompPurity : public TCompBase {

private:	
	CHandle colliderHandle;
	CHandle renderHandle;

	TCompRender* getRender();
	TCompCollider* getCollider();

public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onPurityChange(const TMsgPurityChange & msg);


};
