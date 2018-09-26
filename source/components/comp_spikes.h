#pragma once


struct TMsgEntitiesGroupCreated;
struct TMsgTriggerEnter;
struct TMsgTriggerExit;

class TCompSpikes : public TCompBase {
private:
	float damage = 0.f;
	float invulnerabilityTime = 0.f;
	float stunTime = 0.f;
	std::set<CHandle> handles;

	float hitAgainTime = 0.f;
	CTimer timer;

public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void update(float delta);

	void load(const json& j, TEntityParseContext& ctx);
	void onTriggerEnter(const TMsgTriggerEnter & msg);	
	void onTriggerExit(const TMsgTriggerExit& msg);
};

