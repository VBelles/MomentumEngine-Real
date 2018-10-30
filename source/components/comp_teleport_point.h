#pragma once


struct TMsgEntitiesGroupCreated;
struct TMsgTriggerEnter;

class TCompTeleportPoint : public TCompBase {
private:
	void setActive();
	void setAlreadyActive();

	bool isActivated = false;
	CTimer2 activationTimer;
	float activationTime = 1.5f;

	void changeMaterialToWorking();
	void changeMaterialToPaused();

public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void update(float delta);

	void load(const json& j, TEntityParseContext& ctx);
	void onTriggerEnter(const TMsgTriggerEnter & msg);
	void onGroupCreated(const TMsgEntitiesGroupCreated & msg);
};

