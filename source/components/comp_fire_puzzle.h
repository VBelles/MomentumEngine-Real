#pragma once


class TCompFirePuzzle : public TCompBase {
private:

	int goal = 0;
	std::string chrysalisName;

	std::set<CHandle> fires;
	int turnedOffFires = 0;

	bool done = false;

	void onEntitGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onFireTurnOff(const TMsgFireTurnOff& msg);
public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	//void update(float delta);
};
