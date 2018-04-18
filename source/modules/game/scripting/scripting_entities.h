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
	static int spawnedGolemCount;
public:
	static void create() { if (!instance) instance = new ScriptingEntities(); }
	static void destroy() { SAFE_DELETE(instance) }
	static ScriptingEntities* get() { return instance; }
	static void bind(SLB::Manager* manager);
	std::string spawnEntityAt(std::string prefabFilename, float x, float y, float z);
	static std::string spawnGolemAt(float x, float y, float z);
	static std::string spawnGolem();
	TCompTransform* getPlayerTransform();
};

