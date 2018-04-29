#pragma once

enum LuaCall {
	onGameStart,
	onLevelStart, //param levelId
	onPowerLevelChange, //param powerLevel
	onTriggerEnter, //param triggerName, entityName
	onTriggerExit, //param triggerName, entityName
	onItemDestroyed, //param itemName
	onEnemyKilled, //param enemyName
	onItemCollected, //param itemName
	onPlayerKilled,
	onAltarDestroyed //param altarName
};

namespace SLB {
	class Manager;
	class Script;
}
class SimpleConsole;

class CModuleScripting : public IModule {
private:
	static CModuleScripting* instance;

	struct DelayedCall {
		float startTime;
		float delay;
		std::string func;
		std::string params;

		friend bool operator<(const DelayedCall& dc1, const DelayedCall& dc2) {
			if (dc1.startTime < dc2.startTime) return true;
			else if (dc1.startTime == dc2.startTime) return dc1.delay < dc2.delay;
			else return false;
		}
	};

	SLB::Manager* manager;
	SLB::Script* script;
	SimpleConsole* console;

	std::map<LuaCall, std::string> luaCalls;

	void initConsole();
	void initSLB();

	void execString(const char* string);
	void execFile(const char* string);

	CTimer timer;
	std::set<DelayedCall> delayedCalls;
	void checkDelayedCalls();

	int nextCoroutineId = 0;
	std::string getNextCoroutineId();

public:
	CModuleScripting(const std::string& name);
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;

	void reset();

	void doFile(const char* filename);
	void doFile(std::string filename);

	void throwEvent(LuaCall event, std::string params);

	SimpleConsole* getConsole() { return console; }

	void callDelayed(float delay, const char* func, const char* params);
};

//Callback for LUA print function
void printCallback(SLB::Script* script, const char* str, size_t strSize);