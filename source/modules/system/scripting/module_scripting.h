#pragma once

enum luaCall {
	onGameStart,
	onLevelStart, //param levelId
	onPowerLevelChange, //param powerLevel
	onTriggerEnter, //param triggerName, entityName
	onTriggerExit, //param triggerName, entityName
	onItemDestroyed, //param itemName
	onEnemyKilled, //param enemyName
	onItemCollected, //param itemName
	onPlayerKilled
};

namespace SLB {
	class Manager;
	class Script;
}
class SimpleConsole;

class CModuleScripting : public IModule {
private:
	SLB::Manager* manager;
	SLB::Script* script;
	SimpleConsole* console;

	CTimer timer;

	std::map<luaCall, std::string> luaCalls;

	void execString(const char* string);
	void execFile(const char* string);

	void initConsole();
	void initSLB();

public:
	CModuleScripting(const std::string& name);
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;

	void doFile(const char* filename);
	void doFile(std::string filename);

	void throwEvent(luaCall event, std::string params);

	SimpleConsole* getConsole() { return console; }
};

//Callback for LUA print function
void printCallback(SLB::Script* script, const char* str, size_t strSize);