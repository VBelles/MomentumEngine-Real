#pragma once

namespace SLB {
	class Manager;
}

class ScriptingManager {
private:
	ScriptingManager();
	~ScriptingManager();
	static ScriptingManager* instance;

public:
	static void create() { if (!instance) instance = new ScriptingManager(); }
	static void destroy() { SAFE_DELETE(instance) }
	static ScriptingManager* get() { return instance; }
	static void bind(SLB::Manager* manager);

	static void callDelayed(float delay, const char* func, const char* params);
	static void setPlayerHp(float hp);
};

