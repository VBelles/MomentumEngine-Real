#pragma once

class Respawner;
class EnemyManager;

class CModuleGame : public IModule {
private:
	//GUI::CHudController* hudController;
	Respawner* respawner;
	EnemyManager* enemyManager;
public:
	CModuleGame(const std::string& aname) : IModule(aname) {}
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;
	Respawner* getRespawner();
};
