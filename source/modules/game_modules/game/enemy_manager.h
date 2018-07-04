#pragma once

class Respawner {
private:
	struct Respawnable {
		std::string prefab;
		VEC3 position;
		float respawnTime;
		CTimer respawnTimer;
	};

	std::map<std::string, Respawnable> elementsToSpawn;

	
public:
	Respawner();
	~Respawner();

	void update(float delta);
	void addElementToSpawn(std::string name, std::string prefab, VEC3 position, float respawnTime);
};

