#pragma once

namespace SLB {
	class Manager;
}

class ScriptingManager {
private:

public:
	static void bind(SLB::Manager* manager);
	static void bindGameConstants(SLB::Manager* manager);

	static void callDelayed(float delay, const char* func, const char* params);
};

