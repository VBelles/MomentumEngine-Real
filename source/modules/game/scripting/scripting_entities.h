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
public:
	static void create() { if (!instance) instance = new ScriptingEntities(); }
	static void destroy() { SAFE_DELETE(instance) }
	static ScriptingEntities* get() { return instance; }
	static void bind(SLB::Manager* manager);
	static void spawnGolemAt(float x, float y, float z);
	static void spawnGolem();
	TCompTransform* getPlayerTransform();
};

