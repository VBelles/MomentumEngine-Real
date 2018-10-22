#pragma once

namespace SLB {
	class Manager;
}
class IInterpolator;

class ScriptingCameras {
private:
	static void bindConstants(SLB::Manager* manager);

public:
	static void bind(SLB::Manager* manager);

	static void blendInCamera(std::string cameraName, float timeToMixIn, int priority, std::string interpolator);
	static void copyRotationFromCamToCam(std::string copyCamName, std::string pasteCamName);
	static void startScreenTransition(float startingRatio, float finalRatio, float startingAlpha, float finalAlpha, float delay);
};

