#pragma once

class Respawner;

class CModuleGame : public IModule {
private:
	//GUI::CHudController* hudController;
	Respawner* respawner;
public:
	CModuleGame(const std::string& aname) : IModule(aname) {}
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;
	Respawner* getRespawner();
};
