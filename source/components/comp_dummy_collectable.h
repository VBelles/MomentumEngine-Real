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

	float chrysalisStartingScale = 0.6f;
	float powerupStartingScale = 0.6f;
	float lifePieceStartingScale = 0.6f;
	VEC3 chrysalisPositionOffset = VEC3(0.2f, 1.5f, 1.2f);
	VEC3 powerupPositionOffset = VEC3(0.3f, 1.5f, 1.f);
	VEC3 lifePiecePositionOffset = VEC3(0.3f, 1.5f, 1.1f);
	float chrysalisYawOffset = deg2rad(90);
	float powerupYawOffset = deg2rad(90);
	float lifePieceYawOffset = deg2rad(180);

	bool isActive = false;

	CTimer2 timer;
	float timeToStartRotation = 0.5f;
	float timeToStartScaling = 2.f;
	float timeToScaleZero = 1.f;//los dos tiempos de escalado tiene que sumar 3 aprox
	float rotationAcceleration = 3.f;
	float maxRotationSpeed = 8.f;

	CHandle currentCollectableHandle;

	CHandle chrysalisHandle;
	CHandle powerupHandle;
	CHandle lifePieceHandle;

	CHandle playerTransformHandle;

	//Messages
	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);

	TCompTransform* getPlayerTransform() { return playerTransformHandle; }

public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);

	void update(float delta);
	void activateSequence(DummyCollectableType type);
};
