#pragma once

#include "comp_base.h"
#include "entity/msgs.h"

struct TMsgEntitiesGroupCreated;
struct TMsgTriggerEnter;
struct TMsgColliderDestroyed;

class TCompTransform;

class TCompCollectable : public TCompBase {

public:
	enum Type {
		UNDEFINED, CHRYSALIS, COIN
	};

	std::map<std::string, Type> typeByName = {
		{ "undefined", UNDEFINED },
	    { "chrysalis", CHRYSALIS },
	    { "coin", COIN }
	};
	
private:
	CTimer timer;
	bool collected = false;
	Type type;

	bool hasSelfIllumVariation = false;
	float selfIllumVariationHalfAmplitude = 0.15f;
	float selfIllumVariationTimeFactor = 3.3f;

	float rotationSpeed;
	VEC3 rotationAxis;
	float rotation = 0.f;

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
};

struct TMsgCollect {
	CHandle collectableHandle;
	TCompCollectable::Type type;
	DECL_MSG_ID();
};