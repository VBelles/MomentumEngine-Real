#pragma once


struct TMsgEntitiesGroupCreated;
struct TMsgColliderDestroyed;
struct TMsgAttackHit;

//Breakable only by player
class TCompLightFlicker : public TCompBase {
private:
	CHandle lightHandle;

	float intensityVariation;
	float variationSpeed;
	float originalIntensity;
	float previousIntensity;
	float nextIntensity;
	float ratio;
	bool active = true;

	//Messages
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);

public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float dt);
	void setActive(bool active);
	bool isActive();
};
