#pragma once

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

	SLB::Manager* getManager() { return manager; }
	SLB::Script* getScript() { return script; }
	SimpleConsole* getConsole() { return console; }

};

//Callback for LUA print function
void printCallback(SLB::Script* script, const char* str, size_t strSize);