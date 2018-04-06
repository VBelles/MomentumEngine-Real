#pragma once

#include "comp_base.h"
#include "entity/common_msgs.h"

class TCompMixCameraTrigger : public TCompBase {
	DECL_SIBLING_ACCESS();

private:
	std::string cameraToMix;
	float timeToMixIn;
	float timeToMixOut;
	bool modifyPlayerCameraRotation = true;

public:	
	void debugInMenu();
	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void onCreate(const TMsgEntityCreated& msg);
	void onTriggerEnter(const TMsgTriggerEnter& msg);
	void onTriggerExit(const TMsgTriggerExit& msg);
	void CopyRotationFromMixedCameraToPlayerCamera();
};
