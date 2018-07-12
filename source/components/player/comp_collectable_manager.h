#pragma once
#include "components/comp_base.h"
#include "components/comp_collectable.h"

struct TMsgCollect;
struct TMsgEntityCreated;
//class TCompCollectable;

class TCompCollectableManager : public TCompBase {
private:
	//maps
	std::map<TCompCollectable::Type, int> objectsCollected;
	std::map<TCompCollectable::Type, std::vector<std::string>> uniqueObjectsCollected;

	void onEntityCreated(const TMsgEntityCreated& msg);
	void onCollect(const TMsgCollect& msg);

	float timeToRespawnCoin = 3.f;

public:
	DECL_SIBLING_ACCESS();

	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);

	void addUniqueCollectable(TCompCollectable::Type type, std::string id);
	void addCollectable(TCompCollectable::Type type, int amount);
	int getNumberOfChrysalis();
	int getNumberOfCoins();
	bool spendCoins(int number);

	void clear();
};
