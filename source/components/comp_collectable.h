#pragma once

#include "entity/msgs.h"

struct TMsgEntitiesGroupCreated;
struct TMsgTriggerEnter;
struct TMsgColliderDestroyed;


class TCompCollectable : public TCompBase {

public:
	enum Type {
		UNDEFINED, CHRYSALIS, COIN, LIFEPIECE
	};

	std::map<std::string, Type> typeByName = {
		{ "undefined", UNDEFINED },
		{ "chrysalis", CHRYSALIS },
		{ "coin", COIN },
		{ "life_piece", LIFEPIECE}
	};

private:
	bool collected = false;
	Type type;

	float rotationSpeed;
	VEC3 rotationAxis;

	bool active = true;

	CHandle transformHandle;
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
	Type getTypeByName(std::string name);
	void setActive(bool active);
	bool isActive();
};

struct TMsgCollect {
	CHandle collectableHandle;
	TCompCollectable::Type type;
	DECL_MSG_ID();
};
