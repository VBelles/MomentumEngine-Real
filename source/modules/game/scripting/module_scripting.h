#pragma once

enum luaCalls {
	onGameStart,
	onLevelStart, //param levelId
	onPowerLevelChange, //param powerLevel
	onTriggerEnter, //param triggerName
	onItemDestroyed, //param itemName
	onEnemyKilled, //param enemyName
	onItemCollected //param itemName
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

public:
	CModuleScripting(const std::string& name);
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;

	void doFile(const char* filename);
	void doFile(std::string filename);

	SLB::Manager* getManager() { return manager; }
	SLB::Script* getScript() { return script; }
	SimpleConsole* getConsole() { return console; }

};

//Callback for LUA print function
void printCallback(SLB::Script* script, const char* str, size_t strSize);