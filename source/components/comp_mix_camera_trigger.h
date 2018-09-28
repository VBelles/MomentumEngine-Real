#pragma once


struct TMsgEntityCreated;
struct TMsgTriggerEnter;
struct TMsgTriggerExit;

using namespace Interpolator;

class TCompMixCameraTrigger : public TCompBase {
	DECL_SIBLING_ACCESS();

private:
	std::string cameraToMix;
	float timeToMixIn;
	float timeToMixOut;
	bool modifyPlayerCameraRotation = true;
	//tipo de interpolator salida y entrada por json

	TCubicInInterpolator* cubicInInterpolator;
	TCubicOutInterpolator* cubicOutInterpolator;
	TCubicInOutInterpolator* cubicInOutInterpolator;
	TExpoInInterpolator* expoInInterpolator;
	TExpoOutInterpolator* expoOutInterpolator;
	TExpoInOutInterpolator* expoInOutInterpolator;

public:	
	void debugInMenu();
	static void registerMsgs();
	void load(const json& j, TEntityParseContext& ctx);
	void onCreate(const TMsgEntityCreated& msg);
	void onTriggerEnter(const TMsgTriggerEnter& msg);
	void onTriggerExit(const TMsgTriggerExit& msg);
	void CopyRotationFromMixedCameraToPlayerCamera();
	//void copyParametersFromPlayerCameraToInterpolationCamera(...)
};
