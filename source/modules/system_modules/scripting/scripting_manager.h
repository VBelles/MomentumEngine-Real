#pragma once

namespace SLB {
	class Manager;
}

class ScriptingManager {
private:
	static void bindConstants(SLB::Manager* manager);

public:
	static void bind(SLB::Manager* manager);

	static void callDelayed(float delay, const char* func, const char* params);
};

