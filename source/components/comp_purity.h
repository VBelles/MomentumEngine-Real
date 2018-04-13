#pragma once

#include "comp_base.h"

class TCompRender;
class TCompCollider;
struct TMsgEntitiesGroupCreated;
struct TMsgPurityChange;

// If the player is in lvl power 1, enables collider. Else, disables it.
class TCompPurity : public TCompBase {
	DECL_SIBLING_ACCESS();

private:
	std::string	originalMeshPath = "";
	std::string	originalMaterialPath = "";
	CHandle colliderHandle;
	CHandle renderHandle;

	TCompRender* getRender();
	TCompCollider* getCollider();
	PxRigidDynamic* getRigidDynamic();

public:
	static void registerMsgs();

	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onPurityChange(const TMsgPurityChange & msg);


};
