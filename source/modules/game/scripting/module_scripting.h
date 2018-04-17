#pragma once

namespace SLB {
	class Manager;
	class Script;
}

class CModuleScripting : public IModule {
private:
	SLB::Manager* manager;
	SLB::Script* script;

public:
	CModuleScripting(const std::string& name);
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;

	SLB::Manager* getManager() { return manager; }
	SLB::Script* getScript() { return script; }

};

void printCallback(SLB::Script* script, const char* str, size_t strSize);