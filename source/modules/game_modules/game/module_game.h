#pragma once

#include "gui/controllers/hud_controller.h"

class Respawner;
class EnemyManager;

struct PointOfInterest {
	std::string name;
	CTransform transform;
};
class CModuleGame : public IModule {
private:
	GUI::CHudController* hudController;
	std::vector<PointOfInterest> pointsOfInterest;
	Respawner* respawner;
	EnemyManager* enemyManager;
	bool ambientSoundLaunched = false;
public:
	CModuleGame(const std::string& aname) : IModule(aname) {}
	bool start() override;
	bool stop() override;
	void update(float delta) override;
	void render() override;
	Respawner* getRespawner();
	EnemyManager * getEnemyManager();

	void showChrysalis();
	void showChrysalis(float time);
	void hideChrysalis();
	void showHUD(bool how);
};
