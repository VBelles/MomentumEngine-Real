#pragma once

enum LuaCall {
	onGameStart,
	onPowerLevelChange, //param powerLevel
	onEnemyKilled, //param enemyName
	onPlayerKilled,
	onLevelStart, //param levelId; la call sera onLevelStart_levelId()
	onTriggerEnter, //param triggerName, entityName; la call sera onTriggerEnter_triggerName(entityName)
	onTriggerExit, //param triggerName, entityName; la call sera onTriggerExit_triggerName(entityName)
	onAltarDestroyed, //param altarName; la call sera onAltarDestroyed_altarName()
	onMechanismSystemActivated, //param mechanismSystemName; la call sera onMechanismSystemActivated_mechanismSystemName()
	onMechanismSystemDeactivated //param mechanismSystemName; la call sera onMechanismSystemDeactivated_mechanisSystemmName()
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
			if ((dc1.startTime == dc2.startTime) && (dc1.delay == dc2.delay) && (dc1.func == dc2.func)) return dc1.params < dc2.params;
			else if ((dc1.startTime == dc2.startTime) && (dc1.delay == dc2.delay)) return dc1.func < dc2.func;
			else if (dc1.startTime == dc2.startTime) return dc1.delay < dc2.delay;
			else return dc1.startTime < dc2.startTime;
		}
	};

	SLB::Manager* manager;
	SLB::Script* script;
	SimpleConsole* console;

	std::map<LuaCall, std::string> luaCalls;
	std::set<LuaCall> callsFirstParamOnFunction;

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
