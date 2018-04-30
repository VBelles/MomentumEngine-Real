#pragma once

class TCompTransform;
namespace SLB {
	class Manager;
}

class ScriptingEntities {

private:
	CHandle playerTransformHandle;
	ScriptingEntities();
	~ScriptingEntities();
	static ScriptingEntities* instance;
	std::map<std::string, int> spawnCounters;
public:
	static void create() { if (!instance) instance = new ScriptingEntities(); }
	static void destroy() { SAFE_DELETE(instance) }
	static ScriptingEntities* get() { return instance; }
	static void bind(SLB::Manager* manager);
	static std::string spawnEntityAt(std::string prefabFilename, float x, float y, float z);
	static std::string spawnGolemAt(float x, float y, float z);
	static std::string spawnGolem();
	static std::string spawnBallAt(float x, float y, float z);
	static std::string spawnBall();
	static std::string spawnMedusaAt(float x, float y, float z);
	static std::string spawnMedusa();
	TCompTransform* getPlayerTransform();
};

