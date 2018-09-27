#pragma once

namespace SLB {
	class Manager;
}

class ScriptingEntities {
private:
	static ScriptingEntities* instance;
	std::map<std::string, int> spawnCounters;
	TCompTransform* getPlayerTransform();

	static void bindConstants(SLB::Manager* manager);

public:
	~ScriptingEntities();
	static void create() { if (!instance) instance = new ScriptingEntities(); }
	static void destroy() { safeDelete(instance); }
	static ScriptingEntities* get() { return instance; }
	static void bind(SLB::Manager* manager);

	static std::string spawnEntityAt(std::string prefabFilename, float x, float y, float z);
	static std::string spawnEntity(std::string prefabFilename);
	static std::string spawnGolemAt(float x, float y, float z);
	static std::string spawnGolem();
	static std::string spawnBallAt(float x, float y, float z);
	static std::string spawnBall();
	static std::string spawnMedusaAt(float x, float y, float z);
	static std::string spawnMedusa();

	static void stopEntities();
	static void stopEntitiesCutscene();
	static void resumeEntities();
	static void resumeEntitiesCutscene();
	static void ScriptingEntities::stopEnemies();
	static void ScriptingEntities::resumeEnemies();
};
