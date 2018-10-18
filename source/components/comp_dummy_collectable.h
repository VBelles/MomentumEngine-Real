#pragma once

struct TMsgEntitiesGroupCreated;

enum DummyCollectableType {
	CHRYSALIS, POWERUP, LIFEPIECE
};

class TCompDummyCollectable : public TCompBase {
private:
	//json
	std::string chrysalisName;
	std::string powerupName;
	std::string lifePieceName;

	float chrysalisStartingScale = 1.f;
	float powerupStartingScale = 1.f;
	float lifePieceStartingScale = 1.f;

	bool isActive = false;

	CTimer2 timer;
	float timeToStartRotation = 0.5f;
	float timeToStartScaling = 1.f;
	float rotationAcceleration = 3.f;
	float maxRotationSpeed = 8.f;
	float timeToScaleZero = 1.f;
	VEC3 positionOffset = VEC3(0, 1.2f, 0.5f);

	CHandle currentCollectableHandle;

	CHandle chrysalisHandle;
	CHandle powerupHandle;
	CHandle lifePieceHandle;

	CHandle playerTransformHandle;

	//Messages
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);

	TCompTransform* getTransform() { return playerTransformHandle; }

public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);

	void update(float delta);
	void activateSequence(DummyCollectableType type);
};
