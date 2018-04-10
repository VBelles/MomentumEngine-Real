#pragma once

#include "comp_base.h"
#include "entity/common_msgs.h"

// If the player is in lvl power 1, enables collider. Else, disables it.
class TCompPurity : public TCompBase {
	DECL_SIBLING_ACCESS();

private:
	std::string	originalMeshPath = "";
	std::string	originalMaterialPath = "";

public:
	static void registerMsgs();

	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onPurityChange(const TMsgPurityChange & msg);

};
