#pragma once

#include "comp_base.h"

struct TMsgEntityCreated;
struct TMsgAllScenesCreated;
struct TMsgTriggerEnter;
struct TMsgTriggerExit;

class TCompSuggestCameraPosTrigger : public TCompBase {
	DECL_SIBLING_ACCESS();

private:
	std::string cameraSuggestion;
	bool suggestYaw = false;
	bool suggestPitch = false;
	bool forceDistance = false;
	bool changeCenteringCamera = false;
	float distanceToCamera = 5.5f;

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
