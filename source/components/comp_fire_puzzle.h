#pragma once


class TCompFirePuzzle : public TCompBase {
private:

	std::set<CHandle> fires;
	int goal = 0;
	int turnedOffFires = 0;

	void onEntitGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onFireTurnOff(const TMsgFireTurnOff& msg);

public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	//void update(float delta);
};
