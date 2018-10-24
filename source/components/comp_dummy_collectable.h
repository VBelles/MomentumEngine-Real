#pragma once

struct TMsgEntitiesGroupCreated;

enum DummyCollectableType {
	DUMMY_CHRYSALIS, DUMMY_POWERUP, DUMMY_LIFEPIECE
};

class TCompDummyCollectable : public TCompBase {
private:
	//json
	std::string chrysalisName;
	std::string powerupName;
	std::string lifePieceName;

	float chrysalisStartingScale = 0.45f;
	float powerupStartingScale = 0.4f;
	float lifePieceStartingScale = 0.45f;
	VEC3 chrysalisPositionOffset = VEC3(0.2f, 1.65f, 0.85f);
	VEC3 powerupPositionOffset = VEC3(0.2f, 1.65f, 0.9f);
	VEC3 lifePiecePositionOffset = VEC3(0.2f, 1.65f, 0.75f);
	float chrysalisYawOffset = deg2rad(90);
	float powerupYawOffset = deg2rad(90);
	float lifePieceYawOffset = deg2rad(180);

	bool isActive = false;

	CTimer2 timer;
	float timeToStartRotation = 0.3f;
	float timeToStartScaling = 2.f;
	float timeToScaleZero = 1.f;//los dos tiempos de escalado tiene que sumar 3 aprox
	float rotationAcceleration = 13.5f;
	float maxRotationSpeed = 50.f;

	float elevationSpeed = -0.05f;

	VEC3 positionOffset;
	float currentRotationSpeed = 0;
	float startingScale;

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
