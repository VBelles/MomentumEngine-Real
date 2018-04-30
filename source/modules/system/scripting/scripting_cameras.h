#pragma once

namespace SLB {
	class Manager;
}

class ScriptingCameras {
private:

public:
	static void bind(SLB::Manager* manager);

	static void blendInCamera(std::string cameraName, float timeToMixIn, int priority, std::string interpolator = INTERPOLATOR_LINEAL);
};

