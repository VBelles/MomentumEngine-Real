#pragma once

struct TMsgAllScenesCreated;
struct TMsgTriggerEnter;
struct TMsgTriggerExit;
struct TMsgColliderDestroyed;
struct TMsgChrysalisCollected;

class TCompFinalDoorController : public TCompBase {
private:
	//json
	std::string finalDoorName;

	CHandle finalDoorCollider;
	CHandle finalDoorTransform;

	bool isRotating = false;

public:
	DECL_SIBLING_ACCESS();

	void debugInMenu();
	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void update(float delta);

	void onAllScenesCreated(const TMsgAllScenesCreated& msg);
	void onTriggerEnter(const TMsgTriggerEnter& msg);
	void onColliderDestroyed(const TMsgColliderDestroyed & msg);
	void onTriggerExit(const TMsgTriggerExit& msg);
	void onChrysalisCollected(const TMsgChrysalisCollected& msg);

	//exponer a lua
	void rotateFinalDoor();
};

