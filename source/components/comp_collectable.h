#pragma once

#include "entity/msgs.h"
#include "modules/system_modules/module_uniques.h"

struct TMsgEntitiesGroupCreated;
struct TMsgTriggerEnter;
struct TMsgColliderDestroyed;


class TCompCollectable : public TCompBase {

public:
	std::map<std::string, ElementType> typeByName = {
		{ "undefined", UNDEFINED },
		{ "chrysalis", CHRYSALIS },
		{ "coin", COIN },
		{ "life_piece", LIFEPIECE}
	};

private:
	bool collected = false;
	ElementType type;

	float rotationSpeed;
	VEC3 rotationAxis;

	bool active = true;
	float helpDistance = -1.f;
	bool nearEffectLaunched = false;

	CHandle transformHandle;
	CHandle playerHandle;

	TCompTransform* getTransform();



public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);

	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onTriggerEnter(const TMsgTriggerEnter& msg);
	void onColliderDestroyed(const TMsgColliderDestroyed& msg);

	void collect();
	ElementType getTypeByName(std::string name);
	void setActive(bool active);
	bool isActive();
};

struct TMsgCollect {
	CHandle collectableHandle;
	ElementType type;
	DECL_MSG_ID();
};
