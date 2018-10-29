#pragma once
#include "modules/system_modules/module_uniques.h"

struct TMsgCollect;
struct TMsgEntityCreated;
struct TMsgAllScenesCreated;

//class TCompRender;
//class TCompCollectable;

class TCompCollectableManager : public TCompBase {
private:
	//maps
	std::map<ElementType, int> objectsCollected;
	std::map<ElementType, std::vector<std::string>> uniqueObjectsCollected;

	void onEntityCreated(const TMsgEntityCreated& msg);
	void onCollect(const TMsgCollect& msg);
	void onAllScenesCreated(const TMsgAllScenesCreated& msg);

	float timeToRespawnCoin = 30.f;
	int maxCoins = 144;
	int lifePiecesPerHeart = 1;

	int numberOfChrysalisTaken = 0;
	int numberOfLifePiecesTaken = 0;
	std::vector<std::string> finalDoorChrysalidesNames;
	std::vector<CHandle> finalDoorChrysalides;
	CTimer2 doorChrysalidesTimer;
	float doorChrysalidesTime = 4.3f;
	bool spawnDoorChrysalis = false;
	std::string finalDoorControllerName;
	CHandle finalDoorControllerEntity;

	float showChrysalisTime = 5.f;

	CTimer2 collectTimer;
	bool isCollecting = false;

	float collectPitch = deg2rad(-20.f);
	float collectYaw = deg2rad(-20.f);
	float collectDistance = 2.f;
	VEC2 cameraSpeed = VEC2(1.5f, 1.5f);
	float collectDuration = 3.f;

public:
	DECL_SIBLING_ACCESS();
	int numberOfPowerUpsTaken = 0;

	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);
	void debugInMenu();

	void addUniqueCollectable(ElementType type, std::string id);
	void addCollectable(ElementType type, int amount);
	int getNumberOfChrysalis();
	int getNumberOfCoins();
	int getMaxCoins();
	int getNumberOfLifePieces();
	bool spendCoins(int number);


	void clear();
};
