#pragma once

namespace SLB {
	class Manager;
	class Script;
}
struct SimpleConsole;

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

	void onPrint(SLB::Script * script, const char * str, size_t strSize);

	SLB::Manager* getManager() { return manager; }
	SLB::Script* getScript() { return script; }

};

void printCallback(SLB::Script* script, const char* str, size_t strSize);