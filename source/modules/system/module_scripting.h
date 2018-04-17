#pragma once



namespace SLB {
	struct Manager;
}

class CModuleScripting : public IModule {
private:
	SLB::Manager * manager;

public:
	CModuleScripting(const std::string& name);
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;
};
