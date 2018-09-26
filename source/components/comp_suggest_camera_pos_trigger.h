#pragma once


struct TMsgEntityCreated;
struct TMsgAllScenesCreated;
struct TMsgTriggerEnter;
struct TMsgTriggerExit;

class TCompSuggestCameraPosTrigger : public TCompBase {
	DECL_SIBLING_ACCESS();

private:
	std::string cameraSuggestion;
	bool suggestYaw = false;
	bool hasOppositeYaw = false;
	bool suggestPitch = false;
	bool forceDistance = false;
	bool changeCenteringCamera = false;
	bool forcePositionOnEnter = true;
	float distanceToCamera = 5.5f;
	VEC2 centeringCameraSpeed = {5.f, 5.f};

	float yawFromCamera;
	float pitchFromCamera;

public:	
	void debugInMenu();
	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void onAllScenesCreated(const TMsgAllScenesCreated& msg);
	void onTriggerEnter(const TMsgTriggerEnter& msg);
	void onTriggerExit(const TMsgTriggerExit& msg);
};
