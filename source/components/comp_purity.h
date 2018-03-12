#pragma once

#include "comp_base.h"
#include "entity/common_msgs.h"
#include "entity/entity_parser.h"
#include "comp_hierarchy.h"
#include "components/comp_transform.h"

// If the player is in lvl power 1, enables collider. Else, disables it.
class TCompPurity: public TCompBase {
	DECL_SIBLING_ACCESS();

    CHandle		player;
	std::string	originalMeshPath = "data/meshes/.mesh";
	std::string	originalMaterialPath = "data/materials/piedras1.material";

public:
    void debugInMenu();
    static void registerMsgs();
    void load(const json& j, TEntityParseContext& ctx);

    void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onPowerLvlChange(const TMsgPowerLvlChange& msg);

    void update(float dt);
};
