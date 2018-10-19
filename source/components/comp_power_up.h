#pragma once

#include "entity/msgs.h"

struct TMsgEntitiesGroupCreated;
struct TMsgAllScenesCreated;
struct TMsgTriggerEnter;
struct TMsgColliderDestroyed;

class TCompPlayerModel;

class TCompPowerUp : public TCompBase {
	
private:
	std::string stateToUnlock;
	std::string message;

	CHandle transformHandle;
	CHandle playerModelHandle;
	CHandle playerTransformHandle;
	TCompTransform* getTransform();
	TCompPlayerModel* getPlayerModel();
	TCompTransform* getPlayerTransform();

	float rotationSpeed = 2.5f;

	CTimer2 collectTimer;
	bool isCollecting = false;

	float collectPitch = deg2rad(-20.f);
	float collectYaw = deg2rad(-20.f);
	float collectDistance = 2.f;
	VEC2 cameraSpeed = VEC2(1.5f, 1.5f);
	float collectDuration = 3.f;

public:
	DECL_SIBLING_ACCESS();
	static void registerMsgs();
	void debugInMenu();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);

	void onGroupCreated(const TMsgEntitiesGroupCreated& msg);
	void onAllScenesCreated(const TMsgAllScenesCreated& msg);
	void onTriggerEnter(const TMsgTriggerEnter& msg);
	void onColliderDestroyed(const TMsgColliderDestroyed& msg);
};
