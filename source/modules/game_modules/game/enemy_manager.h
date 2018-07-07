#pragma once

class EnemyManager {
private:
	//tiempos de respawn
	std::map<EnemyType, float> respawnTimes;

public:
	EnemyManager();
	~EnemyManager();

	void update(float delta);
	void onDead(std::string id);
};

