#pragma once
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

	float timeToRespawnCoin = 30.f;
	int maxCoins = 144;
	int lifePiecesPerHeart = 1;

	float showChrysalisTime = 5.f;

	CTimer2 collectTimer;
	bool isCollecting = false;

	float collectPitch = deg2rad(-20.f);
	float collectDistance = 2.f;
	VEC2 cameraSpeed = VEC2(1.5f, 1.5f);
	float collectDuration = 3.f;

public:
	DECL_SIBLING_ACCESS();

	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);
	void debugInMenu();

	void addUniqueCollectable(TCompCollectable::Type type, std::string id);
	void addCollectable(TCompCollectable::Type type, int amount);
	int getNumberOfChrysalis();
	int getNumberOfCoins();
	int getMaxCoins();
	int getNumberOfLifePieces();
	bool spendCoins(int number);

	void clear();
};
