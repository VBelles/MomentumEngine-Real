#pragma once

namespace SLB {
	class Manager;
}
class IInterpolator;

class ScriptingCameras {
private:
	static Interpolator::IInterpolator* getInterpolator(std::string interpolator);

public:
	static void bind(SLB::Manager* manager);

	static void blendInCamera(std::string cameraName, float timeToMixIn, int priority, std::string interpolator = INTERPOLATOR_LINEAL);
};

